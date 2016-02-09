/*----------------------------------------------------------------------------
 * Copyright (c) Microsoft Corp. 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not 
 * use this file except in compliance with the License.  You may obtain a copy 
 * of the License at http://www.apache.org/licenses/LICENSE-2.0  
 *
 * THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED 
 * WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, 
 * MERCHANTABLITY OR NON-INFRINGEMENT. 
 *
 * See the Apache Version 2.0 License for specific language governing 
 * permissions and limitations under the License.
 *  
 * Based on or incorporating material from the following project(s):
 * TINY MT project, available at http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html.
 * @file tinymt32.h
 * 
 *  @brief Tiny Mersenne Twister only 127 bit internal state
 * 
 *  @author Mutsuo Saito (Hiroshima University)
 *  @author Makoto Matsumoto (University of Tokyo)
 * 
 *  Copyright (C) 2011 Mutsuo Saito, Makoto Matsumoto,
 *  Hiroshima University and The University of Tokyo.
 *  All rights reserved.
 * 
 * 
 * For Informational Purposes:
 * 
 * Copyright (c) 2011 Mutsuo Saito, Makoto Matsumoto, Hiroshima
 * University and The University of Tokyo. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Hiroshima University nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * -----------------------------------------------------------------------------------
 * 
 * File: amp_tinymt_rng.h
 * 
 * Implements 32b tiny Mersenne twister pseudo random number generator using C++ AMP
 *------------------------------------------------------------------------------------ */

#pragma once

#include "amp_rand_collection.h"

typedef class _tinymt_state_32
{
public:
	unsigned        mat1;
	unsigned        mat2;
	unsigned        tmat;
} tinymt_state_32;

typedef class _tinymt32_status_t
{
public:
	unsigned status[4];
	tinymt_state_32 state;
} tinymt32_status_t;

struct _tinymt_dc_32
{
	unsigned 		characteristics[4];
	unsigned		type;
	unsigned		id;
	tinymt_state_32 state;
	unsigned		weight;
	unsigned		delta;
};

// This table is output of the dynamic creator for mersenne twister.
// This table is for 32b RNG only.

typedef  tinymt32_status_t tinymt_status_t;
typedef  tinymt_state_32 tinymt_state_t;
typedef  _tinymt_dc_32 tinymt_dc;

#if defined (_M_X64)
    #error ERROR: This sample may not produce valid results on x64/amd64 platform.
#endif 


/// This is the class implementing tinyMT engine
class tinymt
{
static const unsigned s_tinymt_shift0       = 11;
static const unsigned s_tinymt_shift1       = 10;
static const unsigned s_tinymt_min_loop     = 8;
static const unsigned s_tinymt_pre_loop     = 8;
static const unsigned s_tinymt_mask         = 0x7fffffffU;
static const unsigned s_tinymt_single_mask  = 0x3f800000U;

private:
	void next() restrict(amp);

	unsigned temper() restrict(amp);

    tinymt_status_t  status;

public:
	/// Setup state
	void initialize(tinymt_status_t& init, int seed = 0) restrict(amp);

	/// Setup state
	void initialize(int seed = 0) restrict(amp);

	/// Get next unsigned random number
	unsigned next_uint() restrict(amp);

	/// Get next floating point random number between 1.0 - 2.0
	float next_single12() restrict(amp);

	/// Get next floating point random number
	float next_single() restrict(amp);
};


extern _tinymt_dc_32 tinymt_dc_data_32[];


/// This class initialize all the RNG engines
template<int _rank>
class  tinymt_collection : public amp_rand_collection<tinymt, _rank>
{
private:
    tinymt_collection()
    {
    }

public:
	tinymt_collection(const concurrency::extent<_rank> rand_extent, int seed = 0)
		: _base(rand_extent.size() > 65536 ? extent<_rank>(65536) : rand_extent, seed)
	{
		int max_elements = rand_extent.size() > 65536 ? 65536 : rand_extent.size();

		concurrency::array<tinymt_dc, 1> tinymt_dc_data_a(max_elements, tinymt_dc_data_32);
		concurrency::array_view<tinymt_dc, _rank> tinymt_dc_data_av = tinymt_dc_data_a.view_as<_rank>(rand_extent.size() > 65536 ? extent<_rank>(65536) : rand_extent);

		concurrency::array_view<tinymt, _rank> rng_av(m_rng_av);
		parallel_for_each(rand_extent, [=](concurrency::index<_rank> idx) restrict(amp) {
			while (idx[0] > 65536)
			{
				idx -= index<1>(65536);
			}
			tinymt_status_t init = { 0 };
			init.state = tinymt_dc_data_av[idx].state;
			rng_av[idx].initialize(init, seed);
		});
	}    
	
};
