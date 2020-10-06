/*
 * Original work Copyright 2009 - 2010 Kevin Ackley (kackley@gwi.net)
 * Modified work Copyright 2020 - 2020 Andy Maloney <asmaloney@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "SectionHeaders.h"

namespace e57
{
#ifdef E57_DEBUG
   void BlobSectionHeader::dump( int indent, std::ostream &os )
   {
      os << space( indent ) << "sectionId:            " << sectionId << std::endl;
      os << space( indent ) << "sectionLogicalLength: " << sectionLogicalLength << std::endl;
   }
#endif

   CompressedVectorSectionHeader::CompressedVectorSectionHeader()
   {
      /// Double check that header is correct length.  Watch out for RTTI
      /// increasing the size.
      static_assert( sizeof( CompressedVectorSectionHeader ) == 32,
                     "Unexpected size of CompressedVectorSectionHeader" );
   }

   void CompressedVectorSectionHeader::verify( uint64_t filePhysicalSize )
   {
      /// Verify reserved fields are zero. ???  if fileversion==1.0 ???
      for ( unsigned i = 0; i < sizeof( reserved1 ); i++ )
      {
         if ( reserved1[i] != 0 )
         {
            throw E57_EXCEPTION2( E57_ERROR_BAD_CV_HEADER,
                                  "i=" + toString( i ) + " reserved=" + toString( reserved1[i] ) );
         }
      }

      /// Check section length is multiple of 4
      if ( sectionLogicalLength % 4 )
      {
         throw E57_EXCEPTION2( E57_ERROR_BAD_CV_HEADER, "sectionLogicalLength=" + toString( sectionLogicalLength ) );
      }

      /// Check sectionLogicalLength is in bounds
      if ( filePhysicalSize > 0 && sectionLogicalLength >= filePhysicalSize )
      {
         throw E57_EXCEPTION2( E57_ERROR_BAD_CV_HEADER, "sectionLogicalLength=" + toString( sectionLogicalLength ) +
                                                           " filePhysicalSize=" + toString( filePhysicalSize ) );
      }

      /// Check dataPhysicalOffset is in bounds
      if ( filePhysicalSize > 0 && dataPhysicalOffset >= filePhysicalSize )
      {
         throw E57_EXCEPTION2( E57_ERROR_BAD_CV_HEADER, "dataPhysicalOffset=" + toString( dataPhysicalOffset ) +
                                                           " filePhysicalSize=" + toString( filePhysicalSize ) );
      }

      /// Check indexPhysicalOffset is in bounds
      if ( filePhysicalSize > 0 && indexPhysicalOffset >= filePhysicalSize )
      {
         throw E57_EXCEPTION2( E57_ERROR_BAD_CV_HEADER, "indexPhysicalOffset=" + toString( indexPhysicalOffset ) +
                                                           " filePhysicalSize=" + toString( filePhysicalSize ) );
      }
   }

#ifdef E57_DEBUG
   void CompressedVectorSectionHeader::dump( int indent, std::ostream &os ) const
   {
      os << space( indent ) << "sectionId:            " << static_cast<unsigned>( sectionId ) << std::endl;
      os << space( indent ) << "sectionLogicalLength: " << sectionLogicalLength << std::endl;
      os << space( indent ) << "dataPhysicalOffset:   " << dataPhysicalOffset << std::endl;
      os << space( indent ) << "indexPhysicalOffset:  " << indexPhysicalOffset << std::endl;
   }
#endif
}