### Camellia cipher

Based on https://datatracker.ietf.org/doc/html/rfc3713.

#### Build

```bash
git clone https://github.com/delvinru/camellia.git
make
```

#### Usage

```bash
Usage: ./camellia [-ed] [-hv] {-f <file>} [-k <key>] [-l <key_length>] [-o <file>]
-e (--encrypt): encrypt data
-d (--decrypt): decrypt data
-f (--file): input filename
-k (--key): key file for encryption/decryption
-l (--key-len): the length of the key to pad
-o (--output): output filename or use default with postfix '.enc'
-h (--help): show this help page
```
