"""
Python Service Implementation Examples for LibreTap MQTT Protocol
Demonstrates publishing commands and handling events using asyncio-mqtt
"""

import asyncio
import json
from datetime import datetime, UTC
from uuid import uuid4
import asyncio_mqtt as aiomqtt


async def send_auth_start(device_id: str, timeout: int = 30):
    """
    Initiate authentication operation on a device
    
    Args:
        device_id: Target device identifier
        timeout: Operation timeout in seconds
        
    Returns:
        request_id: UUID for tracking this operation
    """
    request_id = str(uuid4())
    
    message = {
        "version": "1.0",
        "timestamp": datetime.now(UTC).isoformat(),
        "device_id": device_id,
        "event_type": "auth_start",
        "request_id": request_id,
        "payload": {
            "timeout_seconds": timeout
        }
    }
    
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.publish(
            f"devices/{device_id}/auth/start",
            payload=json.dumps(message),
            qos=1
        )
    
    return request_id


async def send_auth_verify(device_id: str, request_id: str, tag_uid: str, key: str, user_data: dict):
    """
    Send verification data after tag detection
    
    Args:
        device_id: Target device identifier
        request_id: Original request ID from auth_start
        tag_uid: Detected tag UID
        key: Encryption key for tag verification
        user_data: User context data to verify
    """
    message = {
        "version": "1.0",
        "timestamp": datetime.now(UTC).isoformat(),
        "device_id": device_id,
        "event_type": "auth_verify",
        "request_id": request_id,
        "payload": {
            "tag_uid": tag_uid,
            "key": key,
            "user_data": user_data
        }
    }
    
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.publish(
            f"devices/{device_id}/auth/verify",
            payload=json.dumps(message),
            qos=1
        )


async def send_register_start(device_id: str, tag_uid: str, key: str, timeout: int = 30):
    """
    Initiate tag registration operation
    
    Args:
        device_id: Target device identifier
        tag_uid: Tag UID to register
        key: Encryption key to write to tag
        timeout: Operation timeout in seconds
        
    Returns:
        request_id: UUID for tracking this operation
    """
    request_id = str(uuid4())
    
    message = {
        "version": "1.0",
        "timestamp": datetime.now(UTC).isoformat(),
        "device_id": device_id,
        "event_type": "register_start",
        "request_id": request_id,
        "payload": {
            "tag_uid": tag_uid,
            "key": key,
            "timeout_seconds": timeout
        }
    }
    
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.publish(
            f"devices/{device_id}/register/start",
            payload=json.dumps(message),
            qos=1
        )
    
    return request_id


async def send_read_start(device_id: str, blocks: list, timeout: int = 30):
    """
    Initiate tag read operation
    
    Args:
        device_id: Target device identifier
        blocks: List of block numbers to read
        timeout: Operation timeout in seconds
        
    Returns:
        request_id: UUID for tracking this operation
    """
    request_id = str(uuid4())
    
    message = {
        "version": "1.0",
        "timestamp": datetime.now(UTC).isoformat(),
        "device_id": device_id,
        "event_type": "read_start",
        "request_id": request_id,
        "payload": {
            "timeout_seconds": timeout,
            "read_blocks": blocks
        }
    }
    
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.publish(
            f"devices/{device_id}/read/start",
            payload=json.dumps(message),
            qos=1
        )
    
    return request_id


async def send_cancel(device_id: str, operation: str, request_id: str):
    """
    Cancel an active operation
    
    Args:
        device_id: Target device identifier
        operation: Operation type (register, auth, read)
        request_id: Request ID of operation to cancel
    """
    message = {
        "version": "1.0",
        "timestamp": datetime.now(UTC).isoformat(),
        "device_id": device_id,
        "event_type": f"{operation}_cancel",
        "request_id": request_id,
        "payload": {}
    }
    
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.publish(
            f"devices/{device_id}/{operation}/cancel",
            payload=json.dumps(message),
            qos=1
        )


async def send_reset(device_id: str):
    """
    Reset device to idle state
    
    Args:
        device_id: Target device identifier
    """
    request_id = str(uuid4())
    
    message = {
        "version": "1.0",
        "timestamp": datetime.now(UTC).isoformat(),
        "device_id": device_id,
        "event_type": "reset",
        "request_id": request_id,
        "payload": {}
    }
    
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.publish(
            f"devices/{device_id}/reset",
            payload=json.dumps(message),
            qos=1
        )


async def handle_mqtt_events():
    """
    Subscribe to and handle device events
    
    This example shows basic event routing. In production, integrate with
    your session manager and WebSocket connections.
    """
    async with aiomqtt.Client("mqtt.broker.com") as client:
        await client.subscribe("devices/#")
        
        async for message in client.messages:
            try:
                payload = json.loads(message.payload)
                device_id = payload["device_id"]
                event_type = payload["event_type"]
                request_id = payload["request_id"]
                event_payload = payload["payload"]
                
                # Route events
                if event_type == "auth_tag_detected":
                    # Tag detected, send verify command
                    tag_uid = event_payload["tag_uid"]
                    # In production, fetch key and user_data from database
                    await send_auth_verify(
                        device_id, 
                        request_id, 
                        tag_uid,
                        "0123456789ABCDEF0123456789ABCDEF",
                        {"username": "john.doe", "context": "door_access"}
                    )
                
                elif event_type == "auth_success":
                    # Authentication successful
                    print(f"Auth success for {event_payload['tag_uid']}")
                    # Update session status, notify via WebSocket
                    
                elif event_type == "auth_failed":
                    # Authentication failed
                    print(f"Auth failed: {event_payload['reason']}")
                    # Update session status, notify via WebSocket
                    
                elif event_type == "register_success":
                    # Registration complete
                    print(f"Registered tag {event_payload['tag_uid']}")
                    
                elif event_type == "mode_change":
                    # Device mode changed
                    new_mode = event_payload["mode"]
                    print(f"Device {device_id} mode: {new_mode}")
                    
                elif event_type == "status_change":
                    # Device online/offline
                    status = event_payload["status"]
                    print(f"Device {device_id} status: {status}")
                    
                elif event_type.endswith("_error"):
                    # Error event
                    error = event_payload["error"]
                    error_code = event_payload["error_code"]
                    print(f"Error on {device_id}: {error_code} - {error}")
                    
            except json.JSONDecodeError:
                print(f"Invalid JSON payload: {message.payload}")
            except KeyError as e:
                print(f"Missing required field: {e}")


# Example usage
if __name__ == "__main__":
    # Start event handler
    asyncio.run(handle_mqtt_events())
