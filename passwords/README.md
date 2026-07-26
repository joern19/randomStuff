# Purpose / What does it

Make passwords from `pass` usable in Firefox. So it decrypts passwords from "$PASSWORD_STORE_DIR"/web and encrypts them to the Firefox format.

# Password format and directory structure

The content of a password looks like this (url is optional):
```
<password>

username: alice
created: 2026-07-26
url: https://accounts.google.com
```

If we focus on the /web sufolder, here are some examples that should explain usages:
```
- github.com
- github.com/alt
- work/github.com
- work/example.com/alice
- work/example.com/bob
- a/b/c/example.com/nils
```
