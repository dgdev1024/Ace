/**
 * @file    Ace/System/VirtualFilesystem.cpp
 */

#include <Ace/System/VirtualFilesystem.hpp>

namespace ace
{

    /* Static Members *********************************************************/

    std::vector<VirtualFilesystem::Mount>   VirtualFilesystem::sMounts;

    /* Public Methods *********************************************************/

    void VirtualFilesystem::MountPhysicalDirectory (
        const std::string&  pMountPoint,
        const fs::path&     pRealPath
    )
    {
        std::string lMountPoint = NormalizePath(pMountPoint);
        if (fs::is_directory(pRealPath) == false)
        {
            ACE_THROW(
                std::runtime_error,
                "{}: '{}' is not a directory!",
                "VirtualFilesystem", pRealPath.string()
            );
        }

        sMounts.emplace_back(PhysicalMount { lMountPoint, pRealPath });
    }

    void VirtualFilesystem::MountArchive (
        const std::string&  pMountPoint,
        const fs::path&     pArchivePath
    )
    {
        std::string lMountPoint = NormalizePath(pMountPoint);
        if (fs::exists(pArchivePath) == false)
        {
            ACE_THROW(
                std::runtime_error,
                "{}: Could not find archive file '{}'!",
                "VirtualFilesystem", pArchivePath.string()
            );
        }

        sMounts.emplace_back(ArchiveMount { lMountPoint, pArchivePath });
    }

    std::unique_ptr<IVirtualFile> VirtualFilesystem::OpenFile (
        const std::string&  pLogicalPath
    )
    {
        std::string lLogicalPath = NormalizePath(pLogicalPath);
        for (auto lIter = sMounts.rbegin(); lIter != sMounts.rend(); ++lIter)
        {
            if (auto lFile = AttemptOpen(*lIter, lLogicalPath))
            {
                return lFile;
            }
        }

        return nullptr;
    }

    /* Private Methods ********************************************************/

    std::string VirtualFilesystem::NormalizePath (
        const std::string&  pPath
    )
    {
        static const auto IS_SLASH = [] (const char& lChar)
        {
            return lChar == '/' || lChar == '\\';
        };

        auto lLeft = std::find_if_not(pPath.begin(), pPath.end(), IS_SLASH);
        if (lLeft == pPath.end())
        {
            return "";
        }

        auto lRight = std::find_if_not(pPath.rbegin(),
            std::make_reverse_iterator(lLeft), IS_SLASH).base();

        std::string lPath { lLeft, lRight };
        std::ranges::replace(lPath, '\\', '/');
        return lPath;
    }

    std::unique_ptr<IVirtualFile> VirtualFilesystem::AttemptOpen (
        const Mount&        pMount,
        const std::string&  pLogicalPath
    )
    {
        // Visit the mount union and seek out the file according to the mount
        // type.
        return std::visit(
            [&] (const auto& pVisitedMount) -> std::unique_ptr<IVirtualFile>
            {
                // Get the mount point. Make sure the provided logical path
                // begins with that substring.
                const auto& lMountPoint = pVisitedMount.mMountPoint;
                if (pLogicalPath.starts_with(lMountPoint) == false)
                {
                    return nullptr;
                }

                // Get the remaining subpath after the mount point.
                std::string lSubpath = pLogicalPath.substr(lMountPoint.size());
                if (
                    lSubpath.empty() == false &&
                    (
                        lSubpath[0] == '/' ||
                        lSubpath[0] == '\\'
                    )
                )
                {
                    lSubpath.erase(lSubpath.begin());
                }

                // Depending on the mount structure's type, open the file.
                if constexpr
                    (std::is_same_v<decltype(pVisitedMount), const PhysicalMount&>)
                {
                    auto lRealPath = pVisitedMount.mRealPath / lSubpath;
                    try
                    {
                        return fs::is_regular_file(lRealPath) ?
                            std::make_unique<VirtualLocalFile>(lRealPath) :
                            nullptr;
                    }
                    catch (...)
                    {
                        return nullptr;
                    }
                }
                else if constexpr
                    (std::is_same_v<decltype(pVisitedMount), const ArchiveMount&>)
                {
                    try
                    {
                        return std::make_unique<VirtualArchiveFile>(
                            pVisitedMount.mArchivePath,
                            lSubpath
                        );
                    }
                    catch (std::exception& lEx)
                    {
                        std::printf("%s\n", lEx.what());
                        return nullptr;
                    }
                }

                return nullptr;
            }, pMount
        );
    }

}
