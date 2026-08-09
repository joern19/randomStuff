use std::{ffi::OsStr, path::Path};

use serde::Deserialize;

use crate::entry::Entry;

#[derive(Deserialize)]
struct PassFormat {
  username: String,
  url: Option<String>,
}

// TODO: This is wrong rn.
fn domain_from_path(relative_path: &Path) -> Option<&OsStr> {
  let parent = relative_path.parent()?.file_name();
  if parent == Some(OsStr::new("web")) {
    return relative_path.file_stem();
  }
  parent
}

pub fn pass_file_to_entry(relative_path: &Path, content: String) -> Option<Entry> {
  let domain = match domain_from_path(relative_path) {
    None => {
      eprintln!(
        "Failed to find domain from: {}",
        relative_path.to_string_lossy()
      );
      return None;
    }
    Some(domain) => domain.to_string_lossy().to_string(),
  };

  let (pass, yaml) = content.split_once("\n")?;
  let parsed = match serde_yaml::from_str::<PassFormat>(yaml) {
    Ok(parsed) => parsed,
    Err(err) => {
      eprintln!(
        "Failed to parse {}: {}",
        relative_path.to_string_lossy(),
        err
      );
      return None;
    }
  };

  let url = parsed.url.unwrap_or_else(|| format!("https://{}", domain));
  Some(Entry {
    url,
    password: pass.to_string(),
    username: parsed.username,
    path: relative_path.to_string_lossy().to_string(),
  })
}
