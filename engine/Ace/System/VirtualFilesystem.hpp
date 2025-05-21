/**
 * @file    Ace/System/VirtualFilesystem.hpp
 * @brief   Provides a static class for mounting directories and opening files
 *          through a virtual filesystem (VFS).
 */

#pragma once
#include <Ace/System/VirtualLocalFile.hpp>

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
            std::string pMountPoint,
            fs::path    pRealPath
        );

        /**
         * @brief   Un-mounts all physical directories with the given logical
         *          mount point.
         * 
         * @param   pMountPoint The name of the logical mount point to un-mount.
         */
        static void UnmountPhysicalDirectory (
            const std::string&  pMountPoint
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
        static std::unique_ptr<IVirtualFile> OpenLogicalFile (
            const std::string&  pLogicalPath
        );

        /**
         * @brief   Provides a list of all files found at the given logical
         *          mount point.
         * 
         * @param   pLogicalPath    The logical path to the directory to list.
         * 
         * @return  An `std::vector<std::string>` containing the directory's
         *          logical contents.
         */
        static std::vector<std::string> ListDirectory (
            const std::string&  pLogicalPath
        );

    private:
        static astd::vector_map<std::string, fs::path>  sMounts;    ///< @brief The map of logical mount points.

    };

    using VFS = VirtualFilesystem;

}
