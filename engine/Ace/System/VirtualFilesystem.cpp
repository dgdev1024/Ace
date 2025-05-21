/**
 * @file    Ace/System/VirtualFilesystem.cpp
 */

#include <Ace/System/VirtualFilesystem.hpp>

namespace ace
{

    /* Static Members *********************************************************/

    astd::vector_map<std::string, fs::path>  VirtualFilesystem::sMounts;

    /* Public Methods *********************************************************/

    void VirtualFilesystem::MountPhysicalDirectory (
        std::string pMountPoint,
        fs::path    pRealPath
    )
    {
        // Normalize the mount point string:
        // - Replace any Windows-style slashes '\' with a standard slash '/'.
        // - Remove the leading and trailing separator slashes, if any.
        std::ranges::replace(pMountPoint, '\\', '/');
        if (pMountPoint.empty() == false && pMountPoint.front() == '/')    
            { pMountPoint.erase(pMountPoint.begin()); }
        if (pMountPoint.empty() == false && pMountPoint.back() == '/')
            { pMountPoint.pop_back(); }
        if (pMountPoint.empty() == true)
        {
            ACE_THROW(
                std::invalid_argument,
                "VirtualFilesystem: Attempt to mount physical directory with no mount point!"
            );
        }

        // Ensure that the physical directory provided exists, and is actually
        // a physical directory.
        if (
            fs::exists(pRealPath) == false ||
            fs::is_directory(pRealPath) == false
        )
        {
            ACE_THROW(
                std::runtime_error,
                "VirtualFilesystem: Attempt to mount non-existant physical directory '{}'!",
                pRealPath.string()
            );
        }

        // Store the mount in last-in-first-out (LIFO) order - later mounts are
        // tried first.
        sMounts.emplace_back(std::move(pMountPoint), std::move(pRealPath));
    }

    void VirtualFilesystem::UnmountPhysicalDirectory (
        const std::string&  pMountPoint
    )
    {
        // Find the mount point in the `sMounts` map. Erase it if it exists.
        sMounts.erase(
            std::remove_if(
                sMounts.begin(),
                sMounts.end(),
                [&] (const auto& pMount)
                {
                    return pMount.first == pMountPoint;
                }
            ),
            sMounts.end()
        );
    }

    std::unique_ptr<IVirtualFile> VirtualFilesystem::OpenLogicalFile (
        const std::string&  pLogicalPath
    )
    {
        // Normalize the provided logical path.
        std::string lLogicalPath = pLogicalPath;
        std::ranges::replace(lLogicalPath, '\\', '/');
        if (lLogicalPath.empty() == false && lLogicalPath.front() == '/')    
            { lLogicalPath.erase(lLogicalPath.begin()); }
        if (lLogicalPath.empty() == false && lLogicalPath.back() == '/')
            { lLogicalPath.pop_back(); }
        if (lLogicalPath.empty())
        {
            ACE_THROW(
                std::invalid_argument,
                "VirtualFilesystem: Attempt to open logical file with no filename provided!"
            );
        }

        // Iterate over each mount in reverse order.
        for (auto lIter = sMounts.rbegin(); lIter != sMounts.rend(); ++lIter)
        {
            // Pull the mount point and associated directory from the list.
            const auto& [lMountPoint, lRealPath] = *lIter;

            // Check to see if `lLogicalPath` begins with `lMountPoint` + `/`.
            if (
                lLogicalPath.starts_with(lMountPoint) &&
                (
                    lLogicalPath.size() == lMountPoint.size() ||
                    lLogicalPath[lMountPoint.size()] == '/'
                )
            )
            {
                // Grab the remaining part of the logical path after the mount
                // point. Remove any slashes from the front of that portion.
                std::string lRemainder = lLogicalPath.substr(lMountPoint.size());
                if (lRemainder.empty() == false && lRemainder.front() == '/')
                {
                    lRemainder.erase(lRemainder.begin());
                }

                // Use the modified remainder and the real path to resolve the
                // actual full directory. If that path exists, and resolves to
                // a regular file, open the file and return the handle.
                fs::path lFullPath = lRealPath / lRemainder;
                if (
                    fs::exists(lFullPath) == true &&
                    fs::is_regular_file(lFullPath) == true
                )
                {
                    return std::make_unique<VirtualLocalFile>(lFullPath);
                }
            }
        }

        // File was not found in any of the mounts.
        return nullptr;
    }

    std::vector<std::string> VirtualFilesystem::ListDirectory (
        const std::string&  pLogicalPath
    )
    {
        // Normalize the provided logical path.
        std::string lLogicalPath = pLogicalPath;
        std::ranges::replace(lLogicalPath, '\\', '/');
        if (lLogicalPath.empty() == false && lLogicalPath.front() == '/')    
            { lLogicalPath.erase(lLogicalPath.begin()); }
        if (lLogicalPath.empty() == false && lLogicalPath.back() == '/')
            { lLogicalPath.pop_back(); }
        if (lLogicalPath.empty())
        {
            ACE_THROW(
                std::invalid_argument,
                "VirtualFilesystem: Attempt to perform a directory listing with no path!"
            );
        }

        // This container contains the folder's file contents.
        std::vector<std::string> lFiles;

        // Iterate over each mount in reverse order.
        for (auto lIter = sMounts.rbegin(); lIter != sMounts.rend(); ++lIter)
        {
            // Pull the mount point and associated directory from the list.
            const auto& [lMountPoint, lRealPath] = *lIter;

            // Check to see if `lLogicalPath` begins with `lMountPoint` + `/`.
            if (
                lLogicalPath.starts_with(lMountPoint) &&
                (
                    lLogicalPath.size() == lMountPoint.size() ||
                    lLogicalPath[lMountPoint.size()] == '/'
                )
            )
            {
                // Grab the remaining part of the logical path after the mount
                // point. Remove any slashes from the front of that portion.
                std::string lRemainder = lLogicalPath.substr(lMountPoint.size());
                if (lRemainder.empty() == false && lRemainder.front() == '/')
                {
                    lRemainder.erase(lRemainder.begin());
                }

                // Use the modified remainder and the real path to resolve the
                // actual full directory. If that path exists, and resolves to
                // a physical directory, then list its file contents.
                fs::path lFullPath = lRealPath / lRemainder;
                if (
                    fs::exists(lFullPath) == true &&
                    fs::is_directory(lFullPath) == true
                )
                {
                    for (const auto& lEntry : fs::directory_iterator { lFullPath })
                    {
                        if (lEntry.is_regular_file())
                        {
                            std::string lRelative = lMountPoint;
                            if (lRemainder.empty() == false)
                            {
                                lRelative += '/' + lRemainder;
                            }

                            lRelative += '/' + lEntry.path().filename().string();
                            lFiles.push_back(std::move(lRelative));
                        }
                    }

                    // Once listing is done, break.
                    break;
                }
            }
        }

        return lFiles;
    }

}
