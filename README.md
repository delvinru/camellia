### Camellia cipher

Based on https://datatracker.ietf.org/doc/html/rfc3713.

#### Build

```bash
git clone https://github.com/delvinru/camellia.git
make
```

#### Usage

```bash
Usage: ./camellia [-ed] [-hv] {-f <file>} [-k <key>] [-o <file>]
-e (--encrypt): encrypt data
-d (--decrypt): decrypt data
-f (--file): input filename
-k (--key): key file for encryption/decryption
-o (--output): output filename or empty to out in stdout
-v (--verbose): verbose mode
-h (--help): show this help page
```
