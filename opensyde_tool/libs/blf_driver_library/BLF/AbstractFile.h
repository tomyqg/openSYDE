/*
 * Copyright (C) 2013 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file is part of Tobias Lorenz's Toolkit.
 *
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Tobias Lorenz.
 *
 * GNU General Public License 3.0 Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#pragma once

#include "platform.h"

#include <iostream>

#include "AbstractFile.h"

#include "vector_blf_export.h"

namespace Vector {
namespace BLF {

/** Generic interface to access CompressedFile and UncompressedFile in the same way */
class VECTOR_BLF_EXPORT AbstractFile
{
public:
    /** Read block of data */
    virtual void read(char * s, std::streamsize n) = 0;

    /** Get position in input sequence */
    virtual std::streampos tellg() = 0;

    /** Set position in input sequence */
    virtual void seekg(std::streampos pos) = 0;

    /** Set position in input sequence */
    virtual void seekg(std::streamoff off, std::ios_base::seekdir way) = 0;

    /** Write block of data */
    virtual void write(const char * s, std::streamsize n) = 0;

    /** Get position in input sequence */
    virtual std::streampos tellp() = 0;
};

}
}
