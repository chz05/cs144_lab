#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : 
  capacity_( capacity ), 
  byte_pushed_(0),
  byte_popped_(0),
  byte_buffered_(0), 
  buffer_(),
  view_buffer_()
  {}

void Writer::push( string data )
{ // Your code here.
  if ( closed_ )
  {
    return;
  }
  uint64_t available_capacity = this->available_capacity();
  uint64_t write_size = min( available_capacity, static_cast<uint64_t>( data.size() ) );
  if ( write_size > 0 )
  {
    for ( uint64_t i = 0; i < write_size; i++ ){
      buffer_.push_back( data[i] );
    }
    byte_pushed_ += write_size;
    byte_buffered_ += write_size;
  }

}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

bool Writer::is_closed() const
{
  return closed_; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - byte_buffered_; // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return byte_pushed_; // Your code here.
}

string_view Reader::peek() const
{
  view_buffer_.resize(buffer_.size());  // resize to needed size upfront
  std::copy(buffer_.begin(), buffer_.end(), view_buffer_.begin());
  return string_view(view_buffer_);
}

void Reader::pop( uint64_t len )
{
  if (this->is_finished())
  {
    return;
  }

  uint64_t pop_size = min( len, byte_buffered_ );
  if ( pop_size > 0 )
  {
    for ( uint64_t i = 0; i < pop_size; i++ ){
      buffer_.pop_front();
    }
    byte_popped_ += pop_size;
    byte_buffered_ -= pop_size;
  }
}

bool Reader::is_finished() const
{
  if ( closed_ && byte_buffered_ == 0 )
  {
    return true;
  }
  return false; // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return byte_buffered_; // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return byte_popped_;// Your code here.
}

