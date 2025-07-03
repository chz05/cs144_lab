#include "reassembler.hh"
#include "debug.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  Writer& writer = output_.writer();
  // from 0 to first_unassembled_index are already in the buffer
  uint64_t first_unassembled_index = writer.bytes_pushed();
  // from first_unassembled_index to first_unacceptable_index are in the reassembler's buffer
  uint64_t first_unacceptable_index = first_unassembled_index + writer.available_capacity();

  // Check if we are writing the last byte
  if ( is_last_substring ) {
    is_last = true;
    last_index_ = first_index + data.size();
  }

  //put data into the buffer
  for ( size_t i = 0; i < data.size(); ++i ) {
    uint64_t index = first_index + i;
    if ( index < first_unassembled_index )
      continue; // Already written
    if ( index >= first_unacceptable_index )
      break; // Outside capacity
    if ( !buffer_.count( index ) )
      buffer_[index] = data[i]; // Store if not already present
  }

  // Now we can write the data to the output stream
  std::string to_write;
  while ( buffer_.count( first_unassembled_index ) ) {
    to_write.push_back( buffer_[first_unassembled_index] );
    buffer_.erase( first_unassembled_index );
    ++first_unassembled_index;
  }

  if ( !to_write.empty() ) {
    writer.push( to_write );
  }

  // If we have written the last byte, we can close the stream
  if ( is_last && writer.bytes_pushed() == last_index_ ) {
    writer.close(); // Close the stream if we have written the last byte
  }
}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{
  return buffer_.size();
}
