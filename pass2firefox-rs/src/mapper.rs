use std::{ffi::OsStr, path::Path};

use serde::Deserialize;

use crate::entry::Entry;

#[derive(Deserialize)]
struct PassFormat {
  username: String,
  url: Option<String>,
}

// Simple enough: If it contains a dot, it's probably a domain.
fn domain_from_path(relative_path: &Path) -> Option<&OsStr> {
  if let Some(file_stem) = relative_path.file_stem() && file_stem.to_string_lossy().contains(".") {
    return Some(file_stem);
  }
  for component in relative_path.parent()?.components().rev() {
    let component_os_str = component.as_os_str();
    if component_os_str.to_string_lossy().contains(".") {
      return Some(component_os_str);
    }
  }
  None
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
