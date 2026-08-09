use std::env;
use std::path::PathBuf;

fn main() {
  // Tell cargo to look for shared libraries in the specified directory
  println!("cargo:rustc-link-search=/usr/lib");

  // pkg-config --libs --cflags nss
  println!("cargo:rustc-link-lib=ssl3");
  println!("cargo:rustc-link-lib=smime3");
  println!("cargo:rustc-link-lib=nss3");
  println!("cargo:rustc-link-lib=nssutil3");
  println!("cargo:rustc-link-lib=plds4");
  println!("cargo:rustc-link-lib=plc4");
  println!("cargo:rustc-link-lib=nspr4");
  println!("cargo:rustc-link-lib=gpgme");

  cc::Build::new()
    .file("wrapper.c")
    .include("/usr/include/nss")
    .include("/usr/include/nspr")
    .compile("wrapper");

  let bindings = bindgen::Builder::default()
    // The input header we would like to generate
    // bindings for.
    .header("wrapper.h")
    .allowlist_function("NSS_.*")
    .allowlist_function("PK11.*")
    .allowlist_function("SEC.*")
    .allowlist_function("get_error_c_wrapper")
    .allowlist_function("make_sec_item")
    .allowlist_function("gpgme_.*")
    .allowlist_type("SEC.*")
    .allowlist_type("PK11.*")
    .allowlist_var("SEC.*")
    .allowlist_var("PK11.*")
    .allowlist_var("SECSuccess")
    .allowlist_item("CKM_AES_CBC")
    .layout_tests(false)
    .clang_arg("-I/usr/include/nss")
    .clang_arg("-I/usr/include/nspr")
    // Tell cargo to invalidate the built crate whenever any of the
    // included header files changed.
    .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
    .generate()
    .expect("Unable to generate bindings");

  // Write the bindings to the $OUT_DIR/bindings.rs file.
  let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
  bindings
    .write_to_file(out_path.join("bindings.rs"))
    .expect("Couldn't write bindings!");
}
