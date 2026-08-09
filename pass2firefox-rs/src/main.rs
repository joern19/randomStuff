use std::{fmt::Display, path::Path, process::exit};

use crate::{gpg::Ctx, mapper::pass_file_to_entry, nss::Nss};

mod cerror;
mod entry;
mod gpg;
mod mapper;
mod nss;

use walkdir::WalkDir;

enum ExitCodes {
  Ok = 0,
  Fatal = 1,
  PartialComplete = 2,
}

fn exit_fatal<T, E>(result: Result<T, E>, operation: &'static str) -> T
where
  E: Display + Sized,
{
  match result {
    Ok(r) => r,
    Err(err) => {
      eprintln!("Failed to {}: {}", operation, err);
      exit(ExitCodes::Fatal as i32);
    }
  }
}

fn main() {
  let ctx = exit_fatal(Ctx::make(), "setup gpgme");

  let mut exit_code = ExitCodes::Ok;

  let pass_store_dir = Path::new("/tmp/.password-store");
  let web_pass_dir = pass_store_dir.join("web");
  let mut entries = Vec::new();
  for entry in WalkDir::new(web_pass_dir) {
    let entry = match entry {
      Err(err) => {
        exit_code = ExitCodes::PartialComplete;
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
        exit_code = ExitCodes::PartialComplete;
        eprintln!(
          "Failed to decrypt '{}', skipping it: {}",
          entry.path().to_string_lossy(),
          err,
        );
        continue;
      }
    };
    match pass_file_to_entry(relative_path, output) {
      None => {
        exit_code = ExitCodes::PartialComplete;
      }
      Some(mapped) => entries.push(mapped),
    };
  }

  let a = exit_fatal(serde_json::to_string_pretty(&entries), "serialize json");
  println!("{}", a);
  let nss = exit_fatal(Nss::setup("<redacted>"), "setup nss");

  exit(exit_code as i32); // Not entirely correct, but who cares?
}
