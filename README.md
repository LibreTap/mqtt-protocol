# LibreTap MQTT Protocol Specification

> **Official communication contract between TapService (FastAPI backend) and TapReader (ESP32 NFC devices)**

[![Protocol Version](https://img.shields.io/badge/version-0.1-blue.svg)](CHANGELOG.md)
[![License](https://img.shields.io/badge/license-GPL-green.svg)](LICENSE)

## Overview

This repository defines the MQTT-based protocol for the LibreTap NFC authentication system. It serves as the single source of truth for message formats, topic structures, and communication flows between:

- **TapService**: FastAPI gateway exposing HTTP/WebSocket APIs
- **TapReader**: ESP32-based NFC readers using PN532 modules

### Key Features

- 🔐 **Device-Authoritative**: Devices own state; service synchronizes
- ⚡ **Asynchronous**: Non-blocking command/event pattern
- 🔄 **Idempotent**: Request IDs for deduplication and tracing
- 📊 **Reliable**: QoS 1 for critical messages, QoS 0 for telemetry
- 🔒 **Secure**: TLS 1.3, ACLs, and payload validation

## Quick Links

- [📖 Full Specification](MQTT_PROTOCOL_SPEC.md)
- [�️ Roadmap](ROADMAP.md) - Current v1.0 model & planned v2.0 challenge-response
- [�📋 Documentation Index](PROTOCOL_INDEX.md) - Navigation guide
- [📁 JSON Schemas](schemas/) - Message validation schemas
- [💡 Examples](examples/) - Implementation examples and flow diagrams
- [🔄 Changelog](CHANGELOG.md)
- [💻 Python Implementation](https://github.com/LibreTap/TapService)
- [🔌 ESP32 Implementation](https://github.com/LibreTap/TapReader)

## Operations

| Operation | Description | Two-Way Flow |
|-----------|-------------|--------------|
| **Registration** | Write user credentials to new NFC tag | ✓ |
| **Authentication** | Verify tag and decrypt user data | ✓ (requires verify) |
| **Read** | Read raw tag data | ✗ |
| **Control** | Cancel operations or reset device | ✗ |

## Versioning

This protocol follows semantic versioning:
- **Major**: Breaking changes to message format or flow
- **Minor**: Backward-compatible additions (new event types, fields)
- **Patch**: Documentation clarifications, examples

Current: **v0.1.0**

## License

MIT License - see [LICENSE](LICENSE) for details.