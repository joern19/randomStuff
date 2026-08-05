// use crate::cerror::CError;
use std::{
  // ffi::{c_void, CStr, CString},
  path::Path,
  process::Command,
  // ptr,
};

/*
use pass2firefox_rs::{
  gpgme_ctx_t, gpgme_data_new, gpgme_data_new_from_file, gpgme_data_read, gpgme_data_release,
  gpgme_data_seek, gpgme_data_t, gpgme_new, gpgme_op_decrypt,
  gpgme_protocol_t_GPGME_PROTOCOL_OpenPGP, gpgme_release, gpgme_set_protocol, gpgme_strerror,
  gpgme_strsource,
};*/

/*
fn handle_error(error_code: u32) -> Result<(), CError> {
  if error_code != 0 {
    let c_error_str = unsafe { CStr::from_ptr(gpgme_strerror(error_code)) };
    let c_source_str = unsafe { CStr::from_ptr(gpgme_strsource(error_code)) };
    let c_error = CError {
      msg: format!(
        "{} ({})",
        c_error_str.to_string_lossy(),
        c_source_str.to_string_lossy()
      ),
    };
    Err(c_error)
  } else {
    Ok(())
  }
}

struct GpgData {
  ptr: gpgme_data_t,
}

impl GpgData {
  fn null() -> GpgData {
    GpgData {
      ptr: ptr::null_mut(),
    }
  }
}

impl Drop for GpgData {
  fn drop(&mut self) {
    // SAFETY: If gpgme_data_t is null, this won't break
    unsafe {
      gpgme_data_release(self.ptr);
    }
  }
}

pub struct Ctx {
  ptr: gpgme_ctx_t,
}

impl Ctx {
  pub fn make() -> Result<Ctx, CError> {
    let mut ctx: gpgme_ctx_t = ptr::null_mut();
    let error_code = unsafe { gpgme_new(&mut ctx) };
    handle_error(error_code)?;
    let result = Ctx { ptr: ctx };
    result.configure()?;
    Ok(result)
  }
  fn configure(&self) -> Result<(), CError> {
    let error_code =
      unsafe { gpgme_set_protocol(self.ptr, gpgme_protocol_t_GPGME_PROTOCOL_OpenPGP) };
    handle_error(error_code)
  }

  // Should work 80% of the time. negative values are not checked when seeking, should read this
  // shit like a stream.
  pub fn decrypt(&self, path: String) -> Result<Vec<u8>, CError> {
    let mut data_in = GpgData::null();
    let mut data_out = GpgData::null();

    let path_c_str = CString::new(path).map_err(|_| CError {
      msg: "The path contains a null character.".to_string(),
    })?;
    // WHAT DOES 1 MEAAN?
    handle_error(unsafe { gpgme_data_new_from_file(&mut data_in.ptr, path_c_str.as_ptr(), 1) })?;
    handle_error(unsafe { gpgme_data_new(&mut data_out.ptr) })?;
    handle_error(unsafe { gpgme_op_decrypt(self.ptr, data_in.ptr, data_out.ptr) })?;

    let seek_set = 0; // Seek from beginning of file.
    let seek_end = 2; // Seek from end of file.
    let length = unsafe { gpgme_data_seek(data_out.ptr, 0, seek_end) };
    let length_usize = length as usize;
    unsafe { gpgme_data_seek(data_out.ptr, 0, seek_set) };

    let mut result: Vec<u8> = Vec::with_capacity(length_usize);
    let result_ptr: *mut c_void = result.as_mut_ptr() as *mut c_void;
    let read_bytes = unsafe { gpgme_data_read(data_out.ptr, result_ptr, length_usize) };
    assert_eq!(read_bytes as usize, length_usize);
    unsafe { result.set_len(length_usize) };
    Ok(result)
  }
}

impl Drop for Ctx {
  fn drop(&mut self) {
    unsafe {
      gpgme_release(self.ptr);
    }
  }
}*/

pub fn decrypt(file: &Path) -> Result<String, String> {
  // Ugh, is there a stable gpgme wrapper?
  // I could just wrap it myself again..
  let output = Command::new("gpg")
    .args([
      "--decrypt",
      "--batch",
      "--quiet",
      file.to_string_lossy().to_string().as_str(),
    ])
    .output()
    .map_err(|err| err.to_string())?;

  if !output.status.success() {
    return Err(String::from_utf8_lossy(&output.stderr).to_string());
  }
  String::from_utf8(output.stdout).map_err(|err| err.to_string())
}
