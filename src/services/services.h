#pragma once

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

/* --- NEXTCLOUD --- */
#define NEXTCLOUD_AUTH_URL          "/index.php/login/v2"
#define NEXTCLOUD_AUTH_POLL_DELAY   3000
#define NEXTCLOUD_MAX_AUTH_POLLS    20
#define NEXTCLOUD_DAV_ENDPOINT      "/remote.php/dav/files"

/* --- DISCORD --- */
#define DISCORD_API_ENDPOINT    "/discord"
