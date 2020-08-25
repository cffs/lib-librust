/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Cyril Soldani <cyril.soldani@uliege.be>
 *
 * Copyright (c) 2020, University of Liège. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THIS HEADER MAY NOT BE EXTRACTED OR MODIFIED IN ANY WAY.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <uk/syscall.h>
#include <unistd.h>

off_t lseek64(int fd, off_t offset, int whence) {
    printf("lseek64(%d, %ld, %d)\n", fd, offset, whence);
    return lseek(fd, offset, whence);
}

int lstat64(const char *pathname, struct stat *statbuf) {
    printf("lstat64(\"%s\", _)\n", pathname);
    return lstat(pathname, statbuf);
}

int open64(const char *pathname, int flags, ...) {
    printf("open64(\"%s\", %d)\n", pathname, flags);
    mode_t mode = 0;
    if (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, mode_t);
        va_end(ap);
    }
    return open(pathname, flags, mode);
}

ssize_t pread64(int fd, void *buf, size_t count, off_t offset) {
    printf("pread64(%d, _, %lu, %ld)\n", fd, count, offset);
    return pread(fd, buf, count, offset);
}

ssize_t pwrite64(int fd, const void *buf, size_t count, off_t offset) {
    printf("pwrite64(%d, _, %lu, %ld)\n", fd, count, offset);
    return pwrite(fd, buf, count, offset);
}

int fstat64(int fd, struct stat *statbuf) {
    printf("fstat64(%d, _)\n", fd);
    return fstat(fd, statbuf);
}

int fstatat64(int dirfd, const char *pathname, struct stat *statbuf, int flags)
{
    printf("fstatat64(%d, \"%s\", _, %d)\n", dirfd, pathname, flags);
    return fstatat(dirfd, pathname, statbuf, flags);
}

int ftruncate64(int fd, off_t length) {
    printf("ftruncate64(%d, %ld)\n", fd, length);
    return ftruncate(fd, length);
}

int stat64(const char *pathname, struct stat *statbuf) {
    printf("stat64(\"%s\", _)\n", pathname);
    return stat(pathname, statbuf);
}

static inline long __uk_syscall(long nr, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
	(void) arg1;
	(void) arg2;
	(void) arg3;
	(void) arg4;
	(void) arg5;
	(void) arg6;

	switch (nr) {
	case SYS_uname:
		return uk_syscall_e_uname(arg1);
	case SYS_write:
		return uk_syscall_e_write(arg1, arg2, arg3);
	case SYS_writev:
		return uk_syscall_e_writev(arg1, arg2, arg3);
	case SYS_read:
		return uk_syscall_e_read(arg1, arg2, arg3);
	case SYS_readv:
		return uk_syscall_e_readv(arg1, arg2, arg3);
	case SYS_fstat:
		return uk_syscall_e_fstat(arg1, arg2);
	case SYS_readlink:
		return uk_syscall_e_readlink(arg1, arg2, arg3);
	default:
		uk_pr_debug("syscall \"%s\" is not available\n", uk_syscall_name(nr));
		errno = -ENOSYS;
		return -1;
	}
}

long syscall(long n, ...) {
    printf("syscall(%ld)\n", n);
    va_list ap;
    long a,b,c,d,e,f;
    va_start(ap, n);
    a=va_arg(ap, long);
    b=va_arg(ap, long);
    c=va_arg(ap, long);
    d=va_arg(ap, long);
    e=va_arg(ap, long);
    f=va_arg(ap, long);
    va_end(ap);
    return __uk_syscall(n,a,b,c,d,e,f);
}
