# Schemas Directory

This directory contains JSON schemas for validating MQTT messages in the LibreTap protocol.

## Files

- **`message-envelope.json`** - Base envelope structure for all messages
- **`commands.json`** - Payload schemas for commands (Service → Device)
- **`events.json`** - Payload schemas for events (Device → Service)

## Usage

### Python Validation

```python
import json
import jsonschema

# Load schema
with open('schemas/message-envelope.json') as f:
    envelope_schema = json.load(f)

# Validate message
message = {
    "version": "1.0",
    "timestamp": "2025-11-11T12:00:00.000Z",
    "device_id": "reader-001",
    "event_type": "auth_start",
    "request_id": "550e8400-e29b-41d4-a716-446655440002",
    "payload": {"timeout_seconds": 30}
}

try:
    jsonschema.validate(instance=message, schema=envelope_schema)
    print("Message is valid!")
except jsonschema.ValidationError as e:
    print(f"Invalid message: {e.message}")
```

### JavaScript/TypeScript Validation

```typescript
import Ajv from 'ajv';
import envelopeSchema from './schemas/message-envelope.json';

const ajv = new Ajv();
const validate = ajv.compile(envelopeSchema);

const message = {
    version: "1.0",
    timestamp: "2025-11-11T12:00:00.000Z",
    device_id: "reader-001",
    event_type: "auth_start",
    request_id: "550e8400-e29b-41d4-a716-446655440002",
    payload: { timeout_seconds: 30 }
};

if (validate(message)) {
    console.log("Message is valid!");
} else {
    console.log("Invalid message:", validate.errors);
}
```

## Schema Structure

### Message Envelope

All messages must conform to the base envelope schema, which defines:
- Protocol version
- Timestamp format
- Device ID pattern
- Event type enumeration
- Request ID (UUID v4)
- Payload object

### Command Payloads

Defines schemas for each command type:
- `register_start` - Tag registration command
- `auth_start` - Authentication initiation
- `auth_verify` - Authentication verification
- `read_start` - Tag read command
- `cancel` - Cancel operation
- `reset` - Reset device

### Event Payloads

Defines schemas for each event type:
- `status_change` - Device online/offline
- `mode_change` - Operation mode changes
- `tag_detected` - NFC tag detection
- `success_*` - Operation success events
- `failed_*` - Operation failure events
- `error` - Error events with codes
- `heartbeat` - Device health metrics

## Validation Best Practices

1. **Always validate incoming messages** before processing
2. **Validate outgoing messages** before publishing to catch bugs early
3. **Use strict validation** in production to ensure protocol compliance
4. **Log validation errors** for debugging and monitoring
5. **Keep schemas in sync** with protocol specification

## See Also

- `/examples` - Implementation examples using these schemas
- `MQTT_PROTOCOL_SPEC.md` - Complete protocol specification
