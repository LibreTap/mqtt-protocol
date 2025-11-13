# Changelog

## [Unreleased]

### Added
- Added `unknown` mode value for device error states
- Device can now publish `mode_change` with `mode: "unknown"` when in unrecoverable error state
- Used for hardware failures, corrupted state, or indeterminate device mode

## [0.1.0] - 2025-11-11

### Added
- Initial protocol specification
- Registration, authentication, and read flows
- Error code taxonomy
- Security guidelines
- Python and ESP32 implementation examples
