use std::process::exit;

use crate::{
  gpg::decrypt,
  nss::{Nss, SecItem},
};

mod cerror;
mod gpg;
mod nss;

use serde::Serialize;
use walkdir::WalkDir;

#[derive(Serialize)]
struct Entry {
  path: String,
  data: String,
}

enum ExitCodes {
  Ok = 0,
  Fatal = 1,
  PartialComplete = 2,
}

fn main() {
  /*let ctx = match Ctx::make() {
    Err(err) => {
      eprintln!("Failed to setup gpgme: {}", err);
      exit(ExitCodes::Fatal as i32);
    }
    Ok(ctx) => ctx,
  };*/

  let mut exit_code = ExitCodes::Ok;

  let mut entries = Vec::new();
  for entry in WalkDir::new("/tmp/.password-store") {
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
    if !entry.file_type().is_file() {
      continue;
    }

    if entry.path().extension().and_then(|os_str| os_str.to_str()) != Some("gpg") {
      continue;
    }

    let output = match decrypt(entry.path()) {
      // let output = match ctx.decrypt(entry.path().to_string_lossy().to_string()) {
      // Ok(output) => String::from_utf8_lossy(&output).to_string(),
      Ok(output) => output,
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

    entries.push(Entry {
      path: entry.path().display().to_string(),
      data: output,
    });
  }

  let a = match serde_json::to_string_pretty(&entries) {
    Ok(a) => a,
    Err(err) => {
      eprintln!("AAAHH");
      exit(ExitCodes::Fatal as i32);
    }
  };
  println!("{}", a);

  let nss = match Nss::setup("<redacted>") {
    Err(err) => {
      eprintln!("{}", err.msg);
      exit(1);
    }
    Ok(nss) => nss,
  };

  exit(exit_code as i32); // Not entirely correct, but who cares?
}
