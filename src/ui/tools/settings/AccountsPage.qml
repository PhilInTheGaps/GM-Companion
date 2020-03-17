import QtQuick 2.9
import QtQuick.Controls 2.2
import "accounts"

Column {
    id: general_column
    spacing: 20
    padding: 10

    // Spotify
    SpotifyAccount {
        id: spotify_account
    }
}
