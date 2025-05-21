/**
 * @file    Ace/System/VirtualFilesystem.hpp
 * @brief   Provides a static class for mounting directories and opening files
 *          through a virtual filesystem (VFS).
 */

#pragma once
#include <Ace/System/VirtualLocalFile.hpp>
#include <Ace/System/VirtualArchiveFile.hpp>

namespace ace
{

    /**
     * @brief   A static class used for mounting directories and opening files
     *          in a virtual filesystem (VFS).
     */
    class ACE_API VirtualFilesystem final
    {
    public:

        /**
         * @brief   Mounts a physical directory with the given logical mount
         *          point.
         * 
         * @param   pMountPoint The name of the logical mount point under which 
         *                      given path will be mounted.
         * @param   pRealPath   The actual path to be mounted.
         */
        static void MountPhysicalDirectory (
            const std::string&  pMountPoint,
            const fs::path&     pRealPath
        );

        /**
         * @brief   Mounts a `.zip` archive file with the given logical mount
         *          point.
         * 
         * @param   pMountPoint     The name of the logical mount point under
         *                          which given path will be mounted.
         * @param   pArchivePath    The path to the archive file to mount.
         */
        static void MountArchive (
            const std::string&  pMountPoint,
            const fs::path&     pArchivePath
        );

        /**
         * @brief   Opens a logical file, searching for the file in one of the
         *          mounted directories.
         * 
         * @param   pLogicalPath    The logical path to the file to load.
         * 
         * @return  An `std::unique_ptr` to the opened virtual file if found;
         *          `nullptr` otherwise.
         */
        static std::unique_ptr<IVirtualFile> OpenFile (
            const std::string&  pLogicalPath
        );

    private:
    
        /**
         * @brief   A structure representing a physical directory mount, mapping
         *          a mount point to a real, physical folder path.
         */
        struct PhysicalMount
        {
            std::string mMountPoint;
            fs::path    mRealPath;
        };

        /**
         * @brief   A structure representing an archive mount, mapping a mount
         *          point to a path to a compressed archive (`.zip`) file's
         *          contents.
         */
        struct ArchiveMount
        {
            std::string mMountPoint;
            fs::path    mArchivePath;
        };

        /**
         * @brief   Defines a union representing a mount point.
         */
        using Mount = std::variant<
            PhysicalMount,
            ArchiveMount
        >;

    private:

        /**
         * @brief   Normalizes the given path string, removing any trailing
         *          slashes, and converting any remaining backslashes `\\` to
         *          forward-slashes `/`.
         * 
         * @param   pPath   The path string to normalize.
         * 
         * @return  The normalized string.
         */
        static std::string NormalizePath (
            const std::string&  pPath
        );

        /**
         * @brief   Attempts to open a file in the given mount at the given
         *          logical path.
         * 
         * @param   pMount          The mount to look for the file in.
         * @param   pLogicalPath    The file's logical path string.
         * 
         * @return  An `std::unique_ptr` to the opened virtual file if found;
         *          `nullptr` otherwise.
         */
        static std::unique_ptr<IVirtualFile> AttemptOpen (
            const Mount&        pMount,
            const std::string&  pLogicalPath
        );

    private:
        static std::vector<Mount> sMounts;      ///< @brief The list of mounts.

    };

    using VFS = VirtualFilesystem;

}
