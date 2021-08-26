#pragma once

/* --- GENERAL --- */
#define DEFAULT_SERVER_URL      "https://gm-companion.rophil.lol"

/* --- SPOTIFY --- */
#define LIBRESPOT_DEVICE_NAME   "GM-Companion"
#define SPOTIFY_MAX_REQUESTS    2
#define SPOTIFY_BATCH_SIZE      20
#define SPOTIFY_LOCAL_PORT      59991
#define SPOTIFY_SERVER_PORT     59992
#define SPOTIFY_SCOPE           "user-library-read playlist-read-private streaming user-modify-playback-state user-read-currently-playing user-read-playback-state"

/* --- GOOGLE --- */
#define GOOGLE_LOCAL_PORT       59993
#define GOOGLE_MAX_REQUESTS     5
#define GOOGLE_SCOPE            "https://www.googleapis.com/auth/drive"
#define GOOGLE_MIMETYPE_FOLDER  "application/vnd.google-apps.folder"
#define GOOGLE_MIMETYPE_FILE    "application/vnd.google-apps.file"
#define GOOGLE_MIMETYPE_AUDIO   "application/vnd.google-apps.audio"
#define GOOGLE_DRIVE_CACHE_TIME 60

/* --- NEXTCLOUD --- */
#define NEXTCLOUD_AUTH_URL          "/index.php/login/v2"
#define NEXTCLOUD_AUTH_POLL_DELAY   3000
#define NEXTCLOUD_MAX_AUTH_POLLS    20
#define NEXTCLOUD_DAV_ENDPOINT      "/remote.php/dav/files"

/* --- DISCORD --- */
#define DISCORD_API_ENDPOINT    "/discord"