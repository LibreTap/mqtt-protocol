# Protocol Documentation Index

Quick navigation guide for the LibreTap MQTT Protocol documentation.

## Main Specification

📘 **[MQTT_PROTOCOL_SPEC.md](./MQTT_PROTOCOL_SPEC.md)** - Complete protocol specification
- Topic hierarchy and structure
- Message format and envelope
- Operations and flows (register, auth, read)
- QoS and retention policies
- Error handling and codes
- Security requirements and ACL

## Schemas (JSON Schema Definitions)

📁 **[/schemas](./schemas)** - JSON schemas for message validation

| File | Description |
|------|-------------|
| [message-envelope.json](./schemas/message-envelope.json) | Base envelope structure for all messages |
| [commands.json](./schemas/commands.json) | Service → Device command payload schemas |
| [events.json](./schemas/events.json) | Device → Service event payload schemas |
| [README.md](./schemas/README.md) | Usage guide and validation examples |

## Examples (Implementation & Flow Examples)

📁 **[/examples](./examples)** - Code examples and message flows

### Code Examples

| File | Description |
|------|-------------|
| [python_service.py](./examples/python_service.py) | Python service implementation (asyncio-mqtt) |
| [esp32_device.cpp](./examples/esp32_device.cpp) | ESP32 device implementation (PubSubClient) |

### Flow Examples

| File | Description |
|------|-------------|
| [complete_auth_flow.md](./examples/complete_auth_flow.md) | Complete authentication flow (7 messages) |
| [register_flow.md](./examples/register_flow.md) | Tag registration flow (6 messages) |
| [read_flow.md](./examples/read_flow.md) | Tag read operation flow (5 messages) |
| [README.md](./examples/README.md) | Usage guide for all examples |

## Quick Start

### For Service Developers (Backend)
1. Read [MQTT_PROTOCOL_SPEC.md](./MQTT_PROTOCOL_SPEC.md) sections 1-3
2. Review [python_service.py](./examples/python_service.py) for implementation patterns
3. Use [schemas](./schemas) for message validation
4. Reference [complete_auth_flow.md](./examples/complete_auth_flow.md) for message sequences

### For Device Developers (ESP32/Firmware)
1. Read [MQTT_PROTOCOL_SPEC.md](./MQTT_PROTOCOL_SPEC.md) sections 1-4
2. Review [esp32_device.cpp](./examples/esp32_device.cpp) for implementation patterns
3. Use [schemas](./schemas) to validate your message format
4. Reference flow examples for message sequences

### For Integration Testing
1. Use [schemas](./schemas) to validate all messages
2. Reference flow examples for expected message sequences
3. Review error codes in spec section 6

## Key Concepts

- **Device-Authoritative**: Devices own their state; service synchronizes
- **Asynchronous**: Commands return immediately; results via events
- **Idempotent**: All messages include `request_id` for deduplication
- **Reliable**: QoS 1 for commands/results, QoS 0 for progress

## Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines on contributing to this documentation.

## Changelog

See [CHANGELOG.md](./CHANGELOG.md) for version history and changes.

## License

See [LICENSE](./LICENSE) for license information.
