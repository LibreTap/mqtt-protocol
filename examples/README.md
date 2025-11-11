# Examples Directory

This directory contains implementation examples for the LibreTap MQTT Protocol.

## Files

### Code Examples

- **`python_service.py`** - Python service implementation using asyncio-mqtt
  - Publishing commands to devices
  - Handling device events
  - Complete command functions for all operations

- **`esp32_device.cpp`** - ESP32 device implementation using PubSubClient
  - Publishing events from device
  - Handling commands from service
  - Last Will Testament setup

### Flow Examples

- **`complete_auth_flow.md`** - Complete authentication flow with all messages
- **`register_flow.md`** - Tag registration flow example
- **`read_flow.md`** - Tag read operation flow example

## Usage

These examples demonstrate the complete implementation patterns for both service and device sides of the protocol. Use them as reference when implementing your own MQTT clients.

### Python Service

```python
from examples.python_service import send_auth_start, handle_mqtt_events

# Initiate authentication
request_id = await send_auth_start("reader-001", timeout=30)

# Handle events in background
await handle_mqtt_events()
```

### ESP32 Device

Include the relevant functions from `esp32_device.cpp` in your Arduino/PlatformIO project.

## See Also

- `/schemas` - JSON schemas for message validation
- `MQTT_PROTOCOL_SPEC.md` - Complete protocol specification
