#pragma once

// defines the maximum size in bytes of a single write request to fidoControlPoint. This value SHALL be between 20 and 512
#define FIDO2_CONTROL_POINT_LENGTH 256

// Maximum message size supported by the authenticator.
#define FIDO2_MAX_MSG_SIZE 2048

// Credential ID Length supported by the authenticator.
#define CREDENTIAL_ID_LENGTH 16
