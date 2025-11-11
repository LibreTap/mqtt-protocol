# Complete Authentication Flow Example

This file demonstrates a complete authentication flow with all MQTT messages exchanged between the Service and Device.

## Flow Sequence

1. **Service initiates authentication**
2. **Device changes mode to auth**
3. **Device reports waiting for tag**
4. **User presents tag to reader**
5. **Device detects tag and reports UID**
6. **Service sends verification data**
7. **Device verifies and responds with success/failure**
8. **Device returns to idle mode**

---

## 1. Service → Device: Start Authentication

**Topic:** `devices/reader-001/auth/start`  
**QoS:** 1  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:00.000Z",
  "device_id": "reader-001",
  "event_type": "auth_start",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "timeout_seconds": 30
  }
}
```

---

## 2. Device → Service: Mode Change to Auth

**Topic:** `devices/reader-001/mode`  
**QoS:** 1  
**Retained:** Yes

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:00.100Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "mode": "auth",
    "previous_mode": "idle"
  }
}
```

---

## 3. Device → Service: Waiting for Tag

**Topic:** `devices/reader-001/auth/waiting`  
**QoS:** 0  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:00.200Z",
  "device_id": "reader-001",
  "event_type": "auth_waiting",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "message": "Present tag to reader"
  }
}
```

---

## 4. Device → Service: Tag Detected

**Topic:** `devices/reader-001/auth/tag_detected`  
**QoS:** 1  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:02.000Z",
  "device_id": "reader-001",
  "event_type": "auth_tag_detected",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "message": "Tag detected. Awaiting user data."
  }
}
```

---

## 5. Service → Device: Verify Authentication

**Topic:** `devices/reader-001/auth/verify`  
**QoS:** 1  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:05.000Z",
  "device_id": "reader-001",
  "event_type": "auth_verify",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "key": "0123456789ABCDEF0123456789ABCDEF",
    "user_data": {
      "username": "john.doe",
      "context": "door_access"
    }
  }
}
```

---

## 6. Device → Service: Authentication Success

**Topic:** `devices/reader-001/auth/success`  
**QoS:** 1  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:06.000Z",
  "device_id": "reader-001",
  "event_type": "auth_success",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "authenticated": true,
    "message": "Authentication successful",
    "user_data": {
      "username": "john.doe",
      "context": "door_access"
    }
  }
}
```

---

## 7. Device → Service: Mode Change to Idle

**Topic:** `devices/reader-001/mode`  
**QoS:** 1  
**Retained:** Yes

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:06.100Z",
  "device_id": "reader-001",
  "event_type": "mode_change",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "mode": "idle",
    "previous_mode": "auth"
  }
}
```

---

## Alternative: Authentication Failed

If authentication fails, step 6 would be replaced with:

**Topic:** `devices/reader-001/auth/failed`  
**QoS:** 1  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:06.000Z",
  "device_id": "reader-001",
  "event_type": "auth_failed",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "tag_uid": "04:A1:B2:C3:D4:E5:F6",
    "authenticated": false,
    "reason": "Invalid decryption key"
  }
}
```

---

## Error Scenario

If an error occurs during the flow:

**Topic:** `devices/reader-001/auth/error`  
**QoS:** 1  
**Retained:** No

```json
{
  "version": "1.0",
  "timestamp": "2025-11-11T12:00:03.000Z",
  "device_id": "reader-001",
  "event_type": "auth_error",
  "request_id": "550e8400-e29b-41d4-a716-446655440002",
  "payload": {
    "error": "Tag removed during authentication",
    "error_code": "NFC_TAG_LOST",
    "retry_possible": true,
    "component": "nfc"
  }
}
```

After an error, the device returns to idle mode.
