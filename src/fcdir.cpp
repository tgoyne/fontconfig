/*
 * fontconfig/src/fcdir.c
 *
 * Copyright © 2000 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the author(s) not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors make no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE AUTHOR(S) DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

extern "C" {
#include "fcint.h"
}

#include <algorithm>
#include <filesystem>
#include <vector>

namespace fs = std::tr2::sys;

namespace {
fs::path fc_path(const FcChar8 *p) {
    return fs::path(reinterpret_cast<const char *>(p));
}

template <typename T, typename Del>
auto wrap(T *ptr, Del del) -> std::unique_ptr<T, Del> {
    return std::unique_ptr<T, Del>(ptr, del);
}

FcBool FcFileScanFontConfig(FcFontSet *set, FcBlanks *blanks,
                            const FcChar8 *file, FcConfig *config) {
    int count = 0;
    int id = 0;
    do {
        auto font = wrap(FcFreeTypeQuery(file, id, blanks, &count), FcPatternDestroy);
        if (!font) return FcTrue;

        if (config && !FcConfigSubstitute(config, font.get(), FcMatchScan))
            return FcFalse;
        if (!FcFontSetAdd(set, font.get())) return FcFalse;
        font.release();
    } while (++id < count);
    return FcTrue;
}
}

namespace std { namespace tr2 { namespace sys {
directory_iterator &begin(directory_iterator &it) { return it; }
directory_iterator end(directory_iterator &) {
    return fs::directory_iterator();
}
} } }

extern "C" {

FcBool FcFileIsDir(const FcChar8 *file) { return is_directory(fc_path(file)); }
FcBool FcFileIsLink(const FcChar8 *file) { return is_symlink(fc_path(file)); }
FcBool FcFileIsFile(const FcChar8 *file) { return is_regular(fc_path(file)); }

FcBool FcFileScanConfig(FcFontSet *set, FcStrSet *dirs, FcBlanks *blanks,
                        const FcChar8 *file, FcConfig *config) {
    if (FcFileIsDir(file)) return FcStrSetAdd(dirs, file);
    if (set) return FcFileScanFontConfig(set, blanks, file, config);
    return FcTrue;
}

FcBool FcFileScan(FcFontSet *set, FcStrSet *dirs, FcFileCache *cache FC_UNUSED,
                  FcBlanks *blanks, const FcChar8 *file,
                  FcBool force FC_UNUSED) {
    return FcFileScanConfig(set, dirs, blanks, file, FcConfigGetCurrent());
}

FcBool FcDirScanConfig(FcFontSet *set, FcStrSet *dirs, FcBlanks *blanks,
                       const FcChar8 *dir, FcBool force, FcConfig *config) {
    if (!force) return FcFalse;
    if (!set && !dirs) return FcTrue;
    if (!blanks) blanks = FcConfigGetBlanks(config);

    auto dir_path = fc_path(dir);
    if (!is_directory(dir_path)) return FcTrue;

    std::vector<std::string> files;
    for (auto const &file : fs::directory_iterator(dir_path)) {
        if (is_regular(file.status())) files.push_back(file.path());
    }

    sort(begin(files), end(files));

    for (auto const &file : files)
        FcFileScanConfig(set, dirs, blanks, (const FcChar8 *)file.c_str(),
                         config);

    return FcTrue;
}

FcBool FcDirScan(FcFontSet *set, FcStrSet *dirs, FcFileCache *cache,
                 FcBlanks *blanks, const FcChar8 *dir, FcBool force) {
    if (cache || !force) return FcFalse;

    return FcDirScanConfig(set, dirs, blanks, dir, force, FcConfigGetCurrent());
}

FcCache *FcDirCacheScan(const FcChar8 *dir, FcConfig *config) {
    struct stat dir_stat;
    if (FcStatChecksum(dir, &dir_stat) < 0) return nullptr;

    auto set = wrap(FcFontSetCreate(), FcFontSetDestroy);
    if (!set) return nullptr;

    auto dirs = wrap(FcStrSetCreate(), FcStrSetDestroy);
    if (!dirs) return nullptr;

    if (!FcDirScanConfig(set.get(), dirs.get(), nullptr, dir, FcTrue, config))
        return nullptr;

    auto cache = FcDirCacheBuild(set.get(), dir, &dir_stat, dirs.get());
    if (!cache) return nullptr;

    FcDirCacheWrite(cache, config);

    return cache;
}

FcCache *FcDirCacheRescan(const FcChar8 *dir, FcConfig *config) {
    FcCache *cache = FcDirCacheLoad(dir, config, nullptr);
    if (!cache) return nullptr;

    struct stat dir_stat;
    if (FcStatChecksum(dir, &dir_stat) < 0) return nullptr;

    auto dirs = wrap(FcStrSetCreate(), FcStrSetDestroy);
    if (!dirs) return nullptr;

    if (!FcDirScanConfig(nullptr, dirs.get(), nullptr, dir, FcTrue, config))
        return nullptr;

    auto new_cache = FcDirCacheRebuild(cache, &dir_stat, dirs.get());
    if (!new_cache) return nullptr;

    FcDirCacheUnload(cache);
    FcDirCacheWrite(new_cache, config);

    return new_cache;
}

FcCache *FcDirCacheRead(const FcChar8 *dir, FcBool force, FcConfig *config) {
    FcCache *cache = nullptr;
    if (!force) cache = FcDirCacheLoad(dir, config, nullptr);
    if (!cache) cache = FcDirCacheScan(dir, config);
    return cache;
}

FcBool FcDirSave(FcFontSet *, FcStrSet *, const FcChar8 *) { return FcFalse; }
}
