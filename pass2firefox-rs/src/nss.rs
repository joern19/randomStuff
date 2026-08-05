use crate::cerror::CError;
use std::{
  ffi::{CStr, CString},
  ptr::{self, NonNull},
};

const PR_TRUE: i32 = 1;
const PR_FALSE: i32 = 0;

use pass2firefox_rs::{
  _SECStatus_SECSuccess, get_error_c_wrapper, make_sec_item, NSS_Init, NSS_Shutdown,
  PK11SDR_EncryptWithMechanism, PK11SlotInfo, PK11SlotInfoStr, PK11_Authenticate, PK11_FreeSlot,
  PK11_GetInternalKeySlot, SECITEM_FreeItem, SECItemStr, CKM_AES_CBC,
};

fn handle_status(status: i32) -> Result<(), CError> {
  if status != _SECStatus_SECSuccess {
    Err(get_error())
  } else {
    Ok(())
  }
}

// Use this to make sure nss is initialized
pub struct Nss {}

impl Nss {
  pub fn setup(firefox_dir: &str) -> Result<Nss, CError> {
    let c_string = CString::new(firefox_dir).map_err(|_| CError {
      msg: "The firefox_dir contains an null character that cannot be escaped for C.".to_string(),
    })?;
    let status = unsafe { NSS_Init(c_string.as_ptr()) };
    handle_status(status).map(|_| Nss {})
  }

  pub fn setup_slot(&self) -> Result<Pk11Slot, CError> {
    // SAFETY: Nss is initialized, result is checked for null.
    let raw_slot_ptr = unsafe { PK11_GetInternalKeySlot() };
    let slot = match NonNull::new(raw_slot_ptr) {
      None => return Err(get_error()),
      Some(ptr) => Pk11Slot { _nss: self, ptr },
    };
    let auth_result = unsafe { PK11_Authenticate(slot.ptr.as_ptr(), PR_TRUE, ptr::null_mut()) };
    handle_status(auth_result).map(|_| slot)
  }
}

impl Drop for Nss {
  fn drop(&mut self) {
    let status = unsafe { NSS_Shutdown() };
    if status != _SECStatus_SECSuccess {
      eprintln!("Failed to shutdown NSS!!");
    }
  }
}

fn get_error() -> CError {
  let c_str = unsafe { CStr::from_ptr(get_error_c_wrapper()) };
  CError {
    msg: c_str.to_string_lossy().to_string(),
  }
}

pub struct Pk11Slot<'a> {
  _nss: &'a Nss,
  ptr: NonNull<PK11SlotInfo>,
}

impl Drop for Pk11Slot<'_> {
  fn drop(&mut self) {
    unsafe { PK11_FreeSlot(self.ptr.as_ptr()) }
  }
}

impl Pk11Slot<'_> {
  pub fn as_mut_ptr(&self) -> *mut PK11SlotInfoStr {
    self.ptr.as_ptr()
  }
}

pub trait SecItem {
  fn as_mut_ptr(&mut self) -> *mut SECItemStr;
}

pub struct SecItemCOwned {
  item: SECItemStr,
}

impl Drop for SecItemCOwned {
  fn drop(&mut self) {
    // PR_FALSE: Do not free the item itself (owned by rust), just the data.
    unsafe { SECITEM_FreeItem(self.as_mut_ptr(), PR_FALSE) }
  }
}

impl SecItem for SecItemCOwned {
  fn as_mut_ptr(&mut self) -> *mut SECItemStr {
    &mut self.item
  }
}

impl SecItemCOwned {
  pub fn make_empty() -> SecItemCOwned {
    SecItemCOwned {
      item: unsafe { make_sec_item(ptr::null_mut(), 0) },
    }
  }
}

pub struct SecItemRustOwned {
  data: Box<[u8]>,
  item: SECItemStr,
}

impl SecItem for SecItemRustOwned {
  fn as_mut_ptr(&mut self) -> *mut SECItemStr {
    &mut self.item
  }
}

impl SecItemRustOwned {
  pub fn make(mut data: Box<[u8]>) -> Result<SecItemRustOwned, CError> {
    let len: ::std::os::raw::c_int = data.len().try_into().map_err(|_| CError {
      msg: "Cannot convert size of data to c_int".to_string(),
    })?;
    let ptr = data.as_mut_ptr();
    let item = SecItemRustOwned {
      data,
      item: unsafe { make_sec_item(ptr, len) },
    };
    Ok(item)
  }
}

fn encrypt(slot: &Pk11Slot, data: Box<[u8]>) -> Result<SecItemCOwned, CError> {
  let mut encrypted = SecItemCOwned::make_empty();
  let mut key_id = SecItemCOwned::make_empty();
  let mut data_item = SecItemRustOwned::make(data)?;

  let slot_ptr = slot.as_mut_ptr();
  let key_id_ptr = key_id.as_mut_ptr();
  let mechanism_type = u64::from(CKM_AES_CBC);
  let data_ptr = data_item.as_mut_ptr();
  let encrypted_ptr = encrypted.as_mut_ptr();
  let status = unsafe {
    PK11SDR_EncryptWithMechanism(
      slot_ptr,
      key_id_ptr,
      mechanism_type,
      data_ptr,
      encrypted_ptr,
      ptr::null_mut(),
    )
  };
  handle_status(status).map(|_| encrypted)
}
