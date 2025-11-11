/*
 * ESP32 Device Implementation Examples for LibreTap MQTT Protocol
 * Demonstrates publishing events and handling commands using PubSubClient
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>

// Configuration
const char* DEVICE_ID = "reader-001";
const char* MQTT_BROKER = "mqtt.broker.com";
const int MQTT_PORT = 8883;

// Global state
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
String current_request_id;
int timeout_seconds = 30;


/**
 * Get current timestamp in ISO 8601 format
 */
String getISO8601Timestamp() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    gmtime_r(&now, &timeinfo);
    
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S.000Z", &timeinfo);
    return String(buffer);
}


/**
 * Publish mode change event
 */
void publishModeChange(const char* new_mode, const char* previous_mode) {
    StaticJsonDocument<256> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "mode_change";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["mode"] = new_mode;
    payload["previous_mode"] = previous_mode;
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/mode";
    mqtt_client.publish(topic.c_str(), buffer, true); // retained, QoS 1
}


/**
 * Publish status change event (online/offline)
 */
void publishStatusChange(const char* status, const char* firmware_version, const char* ip_address) {
    StaticJsonDocument<256> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "status_change";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["status"] = status;
    payload["firmware_version"] = firmware_version;
    payload["ip_address"] = ip_address;
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/status";
    mqtt_client.publish(topic.c_str(), buffer, true); // retained, QoS 1
}


/**
 * Publish tag detected event
 */
void publishAuthTagDetected(const char* tag_uid) {
    StaticJsonDocument<256> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "auth_tag_detected";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["tag_uid"] = tag_uid;
    payload["message"] = "Tag detected. Awaiting user data.";
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/auth/tag_detected";
    mqtt_client.publish(topic.c_str(), buffer, false); // QoS 1
}


/**
 * Publish auth success event
 */
void publishAuthSuccess(const char* tag_uid, JsonObject user_data) {
    StaticJsonDocument<512> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "auth_success";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["tag_uid"] = tag_uid;
    payload["authenticated"] = true;
    payload["message"] = "Authentication successful";
    payload["user_data"] = user_data;
    
    char buffer[512];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/auth/success";
    mqtt_client.publish(topic.c_str(), buffer, false); // QoS 1
}


/**
 * Publish auth failed event
 */
void publishAuthFailed(const char* tag_uid, const char* reason) {
    StaticJsonDocument<256> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "auth_failed";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["tag_uid"] = tag_uid;
    payload["authenticated"] = false;
    payload["reason"] = reason;
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/auth/failed";
    mqtt_client.publish(topic.c_str(), buffer, false); // QoS 1
}


/**
 * Publish error event
 */
void publishError(const char* error_msg, const char* error_code, bool retry_possible, const char* component) {
    StaticJsonDocument<256> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "error";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["error"] = error_msg;
    payload["error_code"] = error_code;
    payload["retry_possible"] = retry_possible;
    payload["component"] = component;
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/error";
    mqtt_client.publish(topic.c_str(), buffer, false); // QoS 1
}


/**
 * Publish heartbeat event
 */
void publishHeartbeat(unsigned long uptime, float memory_usage, int operations) {
    StaticJsonDocument<256> doc;
    
    doc["version"] = "1.0";
    doc["timestamp"] = getISO8601Timestamp();
    doc["device_id"] = DEVICE_ID;
    doc["event_type"] = "heartbeat";
    doc["request_id"] = current_request_id;
    
    JsonObject payload = doc.createNestedObject("payload");
    payload["uptime_seconds"] = uptime;
    payload["memory_usage_percent"] = memory_usage;
    payload["operations_completed"] = operations;
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    String topic = String("devices/") + DEVICE_ID + "/heartbeat";
    mqtt_client.publish(topic.c_str(), buffer, false); // QoS 0
}


/**
 * Handle incoming MQTT messages
 */
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
    // Parse JSON payload
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.println("Failed to parse MQTT message");
        return;
    }
    
    String event_type = doc["event_type"];
    current_request_id = doc["request_id"].as<String>();
    
    // Route commands
    if (event_type == "auth_start") {
        timeout_seconds = doc["payload"]["timeout_seconds"];
        
        publishModeChange("auth", "idle");
        
        // Start NFC read loop
        startAuthMode();
    }
    else if (event_type == "auth_verify") {
        String tag_uid = doc["payload"]["tag_uid"];
        String key = doc["payload"]["key"];
        JsonObject user_data = doc["payload"]["user_data"];
        
        // Verify tag with provided key
        bool verified = verifyTag(tag_uid.c_str(), key.c_str());
        
        if (verified) {
            publishAuthSuccess(tag_uid.c_str(), user_data);
        } else {
            publishAuthFailed(tag_uid.c_str(), "Invalid decryption key");
        }
        
        publishModeChange("idle", "auth");
    }
    else if (event_type == "auth_cancel") {
        cancelAuthMode();
        publishModeChange("idle", "auth");
    }
    else if (event_type == "register_start") {
        String tag_uid = doc["payload"]["tag_uid"];
        String key = doc["payload"]["key"];
        timeout_seconds = doc["payload"]["timeout_seconds"];
        
        publishModeChange("register", "idle");
        startRegisterMode(tag_uid.c_str(), key.c_str());
    }
    else if (event_type == "read_start") {
        JsonArray blocks = doc["payload"]["read_blocks"];
        timeout_seconds = doc["payload"]["timeout_seconds"];
        
        publishModeChange("read", "idle");
        startReadMode(blocks);
    }
    else if (event_type == "reset") {
        resetDevice();
        publishModeChange("idle", "");
    }
}


/**
 * Setup MQTT connection
 */
void setupMqtt() {
    mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt_client.setCallback(onMqttMessage);
    
    // Connect with Last Will Testament
    String lwt_topic = String("devices/") + DEVICE_ID + "/status";
    StaticJsonDocument<128> lwt_doc;
    lwt_doc["status"] = "offline";
    char lwt_buffer[128];
    serializeJson(lwt_doc, lwt_buffer);
    
    mqtt_client.connect(DEVICE_ID, lwt_topic.c_str(), 1, true, lwt_buffer);
    
    // Subscribe to device topics
    String subscribe_topic = String("devices/") + DEVICE_ID + "/#";
    mqtt_client.subscribe(subscribe_topic.c_str());
    
    // Publish online status
    publishStatusChange("online", "1.2.3", WiFi.localIP().toString().c_str());
    publishModeChange("idle", "");
}


// Stub functions (implement based on your NFC hardware)
void startAuthMode() { /* TODO */ }
bool verifyTag(const char* tag_uid, const char* key) { /* TODO */ return false; }
void cancelAuthMode() { /* TODO */ }
void startRegisterMode(const char* tag_uid, const char* key) { /* TODO */ }
void startReadMode(JsonArray blocks) { /* TODO */ }
void resetDevice() { /* TODO */ }


void setup() {
    Serial.begin(115200);
    // WiFi setup here...
    setupMqtt();
}


void loop() {
    mqtt_client.loop();
    // Your NFC polling loop here...
}
