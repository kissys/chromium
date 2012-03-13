// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_AUDIO_BUFFER_H_
#define CONTENT_BROWSER_SPEECH_AUDIO_BUFFER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "content/common/content_export.h"

namespace speech {

// Models a chunk derived from an AudioBuffer.
class CONTENT_EXPORT AudioChunk {
 public:
  explicit AudioChunk(int bytes_per_sample);
  AudioChunk(const uint8* data, size_t length, int bytes_per_sample);

  bool IsEmpty() const;
  int bytes_per_sample() const { return bytes_per_sample_; }
  size_t NumSamples() const;
  const std::string& AsString() const;
  int16 GetSample16(size_t index) const;
  const int16* SamplesData16() const;
  friend class AudioBuffer;

 private:
  std::string data_string_;
  int bytes_per_sample_;

  DISALLOW_COPY_AND_ASSIGN(AudioChunk);
};

// Models an audio buffer. The current implementation relies on on-demand
// allocations of AudioChunk(s) (which uses a string as storage).
class AudioBuffer {
 public:
  explicit AudioBuffer(int bytes_per_sample);
  ~AudioBuffer();

  // Enqueues a copy of |length| bytes of |data| buffer.
  void Enqueue(const uint8* data, size_t length);

  // Dequeues, in FIFO order, a single chunk respecting the length of the
  // corresponding Enqueue call (in a nutshell: multiple Enqueue calls followed
  // by Dequeue calls will return the individual chunks without merging them).
  scoped_ptr<AudioChunk> DequeueSingleChunk();

  // Dequeues all previously enqueued chunks, merging them in a single chunk.
  scoped_ptr<AudioChunk> DequeueAll();

  // Removes and frees all the enqueued chunks.
  void Clear();

  // Checks whether the buffer is empty.
  bool IsEmpty() const;

 private:
  typedef ScopedVector<AudioChunk> ChunksContainer;
  ChunksContainer chunks_;
  int bytes_per_sample_;

  DISALLOW_COPY_AND_ASSIGN(AudioBuffer);
};

}  // namespace speech

#endif  // CONTENT_BROWSER_SPEECH_AUDIO_BUFFER_H_
