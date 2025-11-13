# Read Flow Example

Complete message sequence for tag discovery (reading tag UID).

**Note:** In v1.0, the read operation is for **tag discovery only** and returns the `tag_uid` without reading block data. Block data storage and reading will be added in v2.0.

## Flow Sequence

1. **Service sends read start command**
2. **Device changes mode to read**
3. **User presents tag**
4. **Device reads tag UID and reports success**
5. **Device returns to idle**

---

## 1. Service → Device: Start Read

**Topic:** `devices/reader-001/read/start`  
**QoS:** 1

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:00.000Z",
  "device_id": "reader-001",
  "event_type": "read_start",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "timeout_seconds": 30
  }
}
```

---

## 2. Device → Service: Mode Change

**Topic:** `devices/reader-001/mode`  
**QoS:** 1  
**Retained:** Yes

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:00.100Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "mode": "read",
    "previous_mode": "idle"
  }
}
```

---

## 3. Device → Service: Success

**Topic:** `devices/reader-001/read/success`  
**QoS:** 1

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:00.250Z",
  "device_id": "reader-001",
  "event_type": "read_success",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "message": "Tag detected successfully"
  }
}
```

---

## 4. Device → Service: Mode Change to Idle

**Topic:** `devices/reader-001/mode`  
**QoS:** 1  
**Retained:** Yes

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:00.350Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "mode": "idle",
    "previous_mode": "read"
  }
}
```
