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
 * \file np_rayleigh_op.cc
 * \brief Operator for numpy sampling from rayleigh distributions
 */

#include "./np_rayleigh_op.h"
#include "./dist_common.h"

namespace mxnet {
namespace op {

DMLC_REGISTER_PARAMETER(NumpyRayleighParam);

NNVM_REGISTER_OP(_npi_rayleigh)
    .describe("Numpy behavior rayleigh")
    .set_num_inputs([](const nnvm::NodeAttrs& attrs) {
      const NumpyRayleighParam& param = nnvm::get<NumpyRayleighParam>(attrs.parsed);
      int num_inputs                  = 1;
      if (param.scale.has_value()) {
        num_inputs -= 1;
      }
      return num_inputs;
    })
    .set_num_outputs(2)
    .set_attr<nnvm::FNumVisibleOutputs>("FNumVisibleOutputs",
                                        [](const NodeAttrs& attrs) { return 1; })
    .set_attr<nnvm::FListInputNames>("FListInputNames",
                                     [](const NodeAttrs& attrs) {
                                       const NumpyRayleighParam& param =
                                           nnvm::get<NumpyRayleighParam>(attrs.parsed);
                                       int num_inputs = 1;
                                       if (param.scale.has_value()) {
                                         num_inputs -= 1;
                                       }
                                       return (num_inputs == 0)
                                                  ? std::vector<std::string>()
                                                  : std::vector<std::string>{"input1"};
                                     })
    .set_attr_parser(ParamParser<NumpyRayleighParam>)
    .set_attr<mxnet::FInferShape>("FInferShape", TwoparamsDistOpShape<NumpyRayleighParam>)
    .set_attr<nnvm::FInferType>("FInferType",
                                [](const nnvm::NodeAttrs& attrs,
                                   std::vector<int>* in_attrs,
                                   std::vector<int>* out_attrs) {
                                  (*out_attrs)[0] = mshadow::kFloat32;
                                  (*out_attrs)[1] = mshadow::kFloat32;
                                  return true;
                                })
    .set_attr<FResourceRequest>("FResourceRequest",
                                [](const nnvm::NodeAttrs& attrs) {
                                  return std::vector<ResourceRequest>{ResourceRequest::kRandom,
                                                                      ResourceRequest::kTempSpace};
                                })
    .set_attr<FCompute>("FCompute<cpu>", NumpyRayleighForward<cpu>)
    .set_attr<nnvm::FGradient>("FGradient", ElemwiseGradUseInOut{"_backward_broadcast_rayleigh"})
    .add_argument("input1", "NDArray-or-Symbol", "Source input")
    .add_arguments(NumpyRayleighParam::__FIELDS__());

NNVM_REGISTER_OP(_backward_broadcast_rayleigh)
    .set_attr<nnvm::TIsBackward>("TIsBackward", true)
    .set_attr_parser(ParamParser<NumpyRayleighParam>)
    .set_num_inputs([](const nnvm::NodeAttrs& attrs) {
      const NumpyRayleighParam& param = nnvm::get<NumpyRayleighParam>(attrs.parsed);
      int num_inputs                  = 5;
      if (param.scale.has_value())
        num_inputs -= 1;
      return num_inputs;
    })
    .set_num_outputs([](const nnvm::NodeAttrs& attrs) {
      const NumpyRayleighParam& param = nnvm::get<NumpyRayleighParam>(attrs.parsed);
      int num_outputs                 = 1;
      if (param.scale.has_value())
        num_outputs -= 1;
      return num_outputs;
    })
    .set_attr<FResourceRequest>("FResourceRequest",
                                [](const NodeAttrs& attrs) {
                                  return std::vector<ResourceRequest>{ResourceRequest::kTempSpace};
                                })
    .set_attr<FCompute>("FCompute<cpu>", RayleighReparamBackward<cpu>)
    .add_arguments(NumpyRayleighParam::__FIELDS__());

}  // namespace op
}  // namespace mxnet
