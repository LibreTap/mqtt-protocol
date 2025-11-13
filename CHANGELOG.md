# Changelog

## [Unreleased]

### Added
- Added `unknown` mode value for device error states
- Device can now publish `mode_change` with `mode: "unknown"` when in unrecoverable error state
- Used for hardware failures, corrupted state, or indeterminate device mode
- **Section 7.3: Cryptographic Model** - Documented current shared secret authentication implementation
  - Registration: Service generates 128-bit shared secret, device writes to tag's user buffer
  - Authentication: Device validates received key against tag-stored key
  - Security properties and limitations documented
  - Future v2.0 challenge-response model noted (breaking change, not backward compatible)

### Changed
- Clarified authentication flow in examples with shared secret verification notes
- Updated section numbering (7.3→7.4, 7.4→7.5) to accommodate new crypto section
- **Read operation simplified for v1.0:** Tag discovery only (returns `tag_uid` without block data)
  - Removed `read_blocks` parameter from `read_start` command
  - `read/success` returns only `tag_uid` and `message` (no block data)
  - Block data storage/reading deferred to v2.0 roadmap

## [0.1.0] - 2025-11-11

### Added
- Initial protocol specification
- Registration, authentication, and read flows
- Error code taxonomy
- Security guidelines
- Python and ESP32 implementation examples
