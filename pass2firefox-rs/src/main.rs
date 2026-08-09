use std::{
  fmt::Display,
  fs::File,
  io::Write,
  path::Path,
  process::exit,
  sync::atomic::{AtomicBool, Ordering},
};

use crate::{
  gpg::Ctx,
  mapper::pass_file_to_entry,
  nss::{encrypt_encode, Nss},
};

mod cerror;
mod entry;
mod gpg;
mod mapper;
mod nss;

use serde::Serialize;
use walkdir::WalkDir;

fn exit_fatal<T, E>(result: Result<T, E>, operation: &'static str) -> T
where
  E: Display + Sized,
{
  match result {
    Ok(r) => r,
    Err(err) => {
      eprintln!("Failed to {}: {}", operation, err);
      exit(1);
    }
  }
}

static PARTIAL_COMPLETION: AtomicBool = AtomicBool::new(false);

fn get_web_passwords(ctx: &Ctx, pass_store_dir: &Path) -> Vec<entry::Entry> {
  let web_pass_dir = pass_store_dir.join("web");
  let mut entries = Vec::new();
  for entry in WalkDir::new(web_pass_dir) {
    let entry = match entry {
      Err(err) => {
        PARTIAL_COMPLETION.store(true, Ordering::Relaxed);
        let path = err
          .path()
          .map(|path| path.to_string_lossy().to_string())
          .unwrap_or("<unknown>".to_string());
        eprintln!("Failed to walk {} because: {:?}", path, err.to_string());
        continue;
      }
      Ok(entry) => entry,
    };
    let relative_path = exit_fatal(
      entry.path().strip_prefix(pass_store_dir),
      "calculate relative path",
    );
    if !entry.file_type().is_file() {
      continue;
    }

    if entry.path().extension().and_then(|os_str| os_str.to_str()) != Some("gpg") {
      continue;
    }

    let output = match ctx.decrypt(entry.path().to_string_lossy().to_string()) {
      Ok(output) => String::from_utf8_lossy(&output).to_string(),
      Err(err) => {
        PARTIAL_COMPLETION.store(true, Ordering::Relaxed);
        eprintln!(
          "Failed to decrypt '{}', skipping it: {}",
          entry.path().to_string_lossy(),
          err,
        );
        continue;
      }
    };
    match pass_file_to_entry(relative_path, output) {
      None => PARTIAL_COMPLETION.store(true, Ordering::Relaxed),
      Some(mapped) => entries.push(mapped),
    };
  }
  entries
}

#[derive(Serialize)]
struct FirefoxEntry {
  hostname: String,
  #[serde(rename = "encryptedUsername")]
  encrypted_username: String,
  #[serde(rename = "encryptedPassword")]
  encrypted_password: String,
  guid: String,
}

#[derive(Serialize)]
struct FirefoxLogins {
  logins: Vec<FirefoxEntry>,
}

fn main() {
  let mut args = std::env::args();
  let program = args.next().unwrap();
  let firefox_profile = match args.next() {
    None => {
      eprintln!(
        "Usage: {} /home/john/.mozilla/firefox/asdf.default-release",
        program,
      );
      exit(1);
    }
    Some(firefox_dir) => firefox_dir,
  };
  let firefox_profile_path = Path::new(&firefox_profile);

  let ctx = exit_fatal(Ctx::make(), "setup gpgme");

  let pass_store_dir_env = &std::env::var("PASSWORD_STORE_DIR").unwrap();
  let entries = get_web_passwords(&ctx, Path::new(pass_store_dir_env));

  let nss = exit_fatal(
    Nss::setup(&firefox_profile_path.to_string_lossy()),
    "setup nss",
  );
  let nss_slot = exit_fatal(nss.setup_slot(), "setup nss slot");

  let firefox_entries: Vec<FirefoxEntry> = entries
    .iter()
    .map(|entry| {
      // We can fail fataly, since either all work or none.
      FirefoxEntry {
        encrypted_username: exit_fatal(
          encrypt_encode(&nss_slot, &entry.username),
          "encrypt username",
        ),
        encrypted_password: exit_fatal(
          encrypt_encode(&nss_slot, &entry.password),
          "encrypt password",
        ),
        guid: entry.path.clone(),
        hostname: entry.url.clone(),
      }
    })
    .collect();
  let firefox_logins = FirefoxLogins {
    logins: firefox_entries,
  };

  let firefox_logins_json = exit_fatal(
    serde_json::to_string_pretty(&firefox_logins),
    "serialize json",
  );

  let mut output = exit_fatal(
    File::create(firefox_profile_path.join("logins.json")),
    "Failed to create logins.json",
  );
  exit_fatal(
    output.write_all(firefox_logins_json.as_bytes()),
    "write to logins.json",
  );

  exit(if PARTIAL_COMPLETION.load(Ordering::Relaxed) {
    2
  } else {
    0
  });
}
