# json-schema-validator vendored copy

This directory contains the library sources of
[`pboettch/json-schema-validator`](https://github.com/pboettch/json-schema-validator),
used for validating JSON documents (e.g. config files) against a JSON Schema.
Unlike `nlohmann_json`, this library is not header-only: the `.cpp` files
under `src/` must be compiled and linked.

- Upstream version: `2.4.1`
- Retrieved: 2026-11-12
- Local modifications: none (sources are used as published upstream, minus
  `example/`, `test/`, packaging and CI files, which Kotte doesn't need).
- License: MIT; see [`LICENSE`](LICENSE)
- Depends on `nlohmann_json` (already vendored in `vendor/nlohmann_json`).

Include it with `#include <nlohmann/json-schema.hpp>`.

The sources are intentionally pinned. Update them from upstream as a
reviewed dependency change rather than editing them locally.
