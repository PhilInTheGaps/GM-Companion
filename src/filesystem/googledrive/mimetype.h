#pragma once

namespace Files
{
namespace GoogleDrive
{

class MimeType
{
public:
    static constexpr auto FOLDER = "application/vnd.google-apps.folder";
    static constexpr auto FILE = "application/vnd.google-apps.file";
    static constexpr auto UNKNOWN = "application/vnd.google-apps.unknown";
    static constexpr auto AUDIO = "application/vnd.google-apps.audio";
    static constexpr auto VIDEO = "application/vnd.google-apps.video";
    static constexpr auto PHOTO = "application/vnd.google-apps.photo";
};

} // namespace GoogleDrive
} // namespace Files
