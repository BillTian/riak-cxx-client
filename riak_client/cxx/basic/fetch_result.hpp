/*  
 Copyright 2011 Basho Technologies, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef RIAKCXX_FETCH_RESULT_HPP_
#define RIAKCXX_FETCH_RESULT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/error.hpp>
#include <riak_client/cxx/object.hpp>

namespace riak { 

class RIAKC_API fetch_result : public riak_version
{
public:
    explicit fetch_result() { }
    explicit fetch_result(const riak_version& version)
        : riak_version(version) { }
    fetch_result(const riak_version& version, const content_vector& contents)
        : riak_version(version), contents_(contents) { }
public:
    object_ptr    choose(const riak_content& content)
    {
        return object_ptr(new riak::object(*this, content));
    }
    object_ptr    choose_sibling(std::size_t idx)
    {
        if (idx > (contents_.size() - 1))
            throw riak::exception("invalid sibling index");
        return choose(contents_[idx]);
    }
    bool          empty() const { return contents_.empty(); }
    std::size_t   sibling_count() const { return contents_.size(); }
    const content_vector& contents() const { return contents_; }
    operator object() 
    {
        if (contents_.size() != 1) 
        {
            throw riak::exception("unresolved conflict");
        }
        return object(*this, contents_[0]);
    }
    operator const object() const 
    { 
        if (contents_.size() != 1) 
        {
            throw riak::exception("unresolved conflict");
        }
        return object(*this, contents_[0]);
    }
private:
    content_vector contents_;
};

} // ::riak


#endif // include guard
