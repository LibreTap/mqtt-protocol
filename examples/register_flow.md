# Registration Flow Example

Complete message sequence for registering a new NFC tag.

## Flow Sequence

1. **Service sends register start command**
2. **Device changes mode to register**
3. **Device reports waiting for tag**
4. **User presents tag**
5. **Device writes to tag**
6. **Device reports success**
7. **Device returns to idle**

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

## 3. Device → Service: Waiting

**Topic:** `devices/reader-001/register/waiting`  
**QoS:** 0

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T14:00:00.200Z",
  "device_id": "reader-001",
  "event_type": "register_waiting",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "message": "Present tag to reader"
  }
}
```

---

## 4. Device → Service: Writing

**Topic:** `devices/reader-001/register/writing`  
**QoS:** 0

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T14:00:05.000Z",
  "device_id": "reader-001",
  "event_type": "register_writing",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "message": "Writing to tag..."
  }
}
```

---

## 5. Device → Service: Success

**Topic:** `devices/reader-001/register/success`  
**QoS:** 1

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T14:00:07.000Z",
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

## 6. Device → Service: Mode Change to Idle

**Topic:** `devices/reader-001/mode`  
**QoS:** 1  
**Retained:** Yes

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T14:00:07.100Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "650e8400-e29b-41d4-a716-446655440003",
  "payload": {
    "mode": "idle",
    "previous_mode": "register"
  }
}
```
