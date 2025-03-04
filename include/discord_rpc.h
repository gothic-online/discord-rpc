#pragma once
#include <stdint.h>

// clang-format off

#if defined(DISCORD_DYNAMIC_LIB)
#  if defined(_WIN32)
#    if defined(DISCORD_BUILDING_SDK)
#      define DISCORD_EXPORT __declspec(dllexport)
#    else
#      define DISCORD_EXPORT __declspec(dllimport)
#    endif
#  else
#    define DISCORD_EXPORT __attribute__((visibility("default")))
#  endif
#else
#  define DISCORD_EXPORT
#endif

// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DiscordButton {
    uint8_t active;
    char label[129]; // max 128 bytes + null terminatior = 129
    char url[513];   // max 512 bytes + null terminator = 513
} DiscordButton;

typedef struct DiscordRichPresence {
    char state[129];   // max 128 bytes + null terminator = 129
    char details[129]; // max 128 bytes + null terminator = 129
    int64_t startTimestamp;
    int64_t endTimestamp;
    char largeImageKey[33];   // max 32 bytes + null terminator = 33
    char largeImageText[129]; // max 128 bytes + null terminator = 129
    char smallImageKey[33];   // max 32 bytes + null terminator = 33
    char smallImageText[129]; // max 128 bytes + null terminator = 129
    char partyId[129];        // max 128 bytes + null terminator = 129
    int partySize;
    int partyMax;
    int partyPrivacy;
    char matchSecret[129];    // max 128 bytes + null terminator = 129
    char joinSecret[129];     // max 128 bytes + null terminator = 129
    char spectateSecret[129]; // max 128 bytes + null terminator = 129
    int8_t instance;
    DiscordButton button1;
    DiscordButton button2;
} DiscordRichPresence;

typedef struct DiscordUser {
    char userId[32]; // snowflake (64bit int), turned into a ascii decimal string, at most 20 chars +1 null terminator = 21
    char username[344]; // 32 unicode glyphs is max name size => 4 bytes per glyph in the worst case, +1 for null terminator = 129
    char discriminator[8]; // 4 decimal digits + 1 null terminator = 5
    char avatar[128]; // optional 'a_' + md5 hex digest (32 bytes) + null terminator = 35
} DiscordUser;

typedef struct DiscordEventHandlers {
    void (*ready)(const DiscordUser* request);
    void (*disconnected)(int errorCode, const char* message);
    void (*errored)(int errorCode, const char* message);
    void (*joinGame)(const char* joinSecret);
    void (*spectateGame)(const char* spectateSecret);
    void (*joinRequest)(const DiscordUser* request);
} DiscordEventHandlers;

#define DISCORD_REPLY_NO 0
#define DISCORD_REPLY_YES 1
#define DISCORD_REPLY_IGNORE 2
#define DISCORD_PARTY_PRIVATE 0
#define DISCORD_PARTY_PUBLIC 1

DISCORD_EXPORT void Discord_Initialize(const char* applicationId,
                                       DiscordEventHandlers* handlers,
                                       int autoRegister,
                                       const char* optionalSteamId);
DISCORD_EXPORT void Discord_Shutdown(void);

/* checks for incoming messages, dispatches callbacks */
DISCORD_EXPORT void Discord_RunCallbacks(void);

/* If you disable the lib starting its own io thread, you'll need to call this from your own */
#ifdef DISCORD_DISABLE_IO_THREAD
DISCORD_EXPORT void Discord_UpdateConnection(void);
#endif

DISCORD_EXPORT void Discord_UpdatePresence(const DiscordRichPresence* presence);
DISCORD_EXPORT void Discord_ClearPresence(void);

DISCORD_EXPORT void Discord_Respond(const char* userid, /* DISCORD_REPLY_ */ int reply);

DISCORD_EXPORT void Discord_UpdateHandlers(DiscordEventHandlers* handlers);

#ifdef __cplusplus
} /* extern "C" */
#endif
