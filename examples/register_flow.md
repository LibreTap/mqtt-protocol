# Registration Flow Example

Complete message sequence for registering a new NFC tag.

## Flow Sequence

1. **Service sends register start command**
2. **Device changes mode to register**
3. **User presents tag (included in start command)**
4. **Device writes to tag**
5. **Device reports success**
6. **Device returns to idle**

---

## 1. Service → Device: Start Registration

**Topic:** `devices/reader-001/register/start`  
**QoS:** 1

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T14:00:00.000Z",
  "device_id": "reader-001",
  "event_type": "register_start",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "key": "0123456789ABCDEF0123456789ABCDEF",
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
  "timestamp": "2025-11-11T14:00:00.100Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "mode": "register",
    "previous_mode": "idle"
  }
}
```

---

## 3. Device → Service: Success

**Topic:** `devices/reader-001/register/success`  
**QoS:** 1

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T14:00:00.250Z",
  "device_id": "reader-001",
  "event_type": "register_success",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "blocks_written": 8,
    "message": "Tag registered successfully"
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
  "timestamp": "2025-11-11T14:00:00.350Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "mode": "idle",
    "previous_mode": "register"
  }
}
```
