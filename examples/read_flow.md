# Read Flow Example

Complete message sequence for reading data from an NFC tag.

## Flow Sequence

1. **Service sends read start command**
2. **Device changes mode to read**
3. **Device reports waiting for tag**
4. **User presents tag**
5. **Device reads blocks and reports success**
6. **Device returns to idle**

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
    "timeout_seconds": 30,
    "read_blocks": [0, 1, 2, 3, 4, 5, 6, 7]
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

## 3. Device → Service: Waiting

**Topic:** `devices/reader-001/read/waiting`  
**QoS:** 0

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:00.200Z",
  "device_id": "reader-001",
  "event_type": "read_waiting",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "message": "Present tag to reader"
  }
}
```

---

## 4. Device → Service: Success

**Topic:** `devices/reader-001/read/success`  
**QoS:** 1

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:03.000Z",
  "device_id": "reader-001",
  "event_type": "read_success",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "blocks_read": 8,
    "data": {
      "0": "00112233445566778899AABBCCDDEEFF",
      "1": "FFEEDDCCBBAA99887766554433221100",
      "2": "0123456789ABCDEF0123456789ABCDEF",
      "3": "FEDCBA9876543210FEDCBA9876543210",
      "4": "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHH",
      "5": "11111111222222223333333344444444",
      "6": "5555555566666666777777778888888",
      "7": "99999999AAAAAAAABBBBBBBBCCCCCCCC"
    },
    "message": "Tag read successfully"
  }
}
```

---

## 5. Device → Service: Mode Change to Idle

**Topic:** `devices/reader-001/mode`  
**QoS:** 1  
**Retained:** Yes

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T15:00:03.100Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "750e8400-e29b-41d4-a716-446655440004",
  "payload": {
    "mode": "idle",
    "previous_mode": "read"
  }
}
```
