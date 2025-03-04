/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \file mkldnn_reshape-inl.h
 * \brief Function definition of mkldnn reshape operator
 */

#ifndef MXNET_OPERATOR_NN_MKLDNN_MKLDNN_RESHAPE_INL_H_
#define MXNET_OPERATOR_NN_MKLDNN_MKLDNN_RESHAPE_INL_H_

#if MXNET_USE_ONEDNN == 1
#include <vector>

#include "../../tensor/matrix_op-inl.h"

#include "mkldnn_base-inl.h"

namespace mxnet {
namespace op {

class MKLDNNReshapeFwd {
 protected:
  std::shared_ptr<mkldnn::memory> out_;
  std::shared_ptr<mkldnn::memory> temp_;
  std::vector<mkldnn::primitive> prims_;

 public:
  MKLDNNReshapeFwd(const OpReqType& req, const NDArray& input, const NDArray& output);
  int GetWorkspaceSize();
  void Execute(const NDArray& input,
               const NDArray& output,
               const OpReqType& req,
               void* workspace = nullptr);
};

typedef OpSignature MKLDNNReshapeSignature;
MKLDNNReshapeFwd& GetReshapeForward(const OpReqType& req,
                                    const NDArray& input,
                                    const NDArray& output);
}  // namespace op
}  // namespace mxnet

#endif  // MXNET_USE_ONEDNN == 1
#endif  // MXNET_OPERATOR_NN_MKLDNN_MKLDNN_RESHAPE_INL_H_
