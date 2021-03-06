/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
//  @author raver119@gmail.com
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_bincount)

//#include <ops/declarable/headers/parity_ops.h>
#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/weights.h>

namespace nd4j {
    namespace ops {
        DECLARE_TYPES(bincount) {
            getOpDescriptor()
                    ->setAllowedInputTypes(nd4j::DataType::ANY)
                    ->setAllowedOutputTypes({ALL_FLOATS});
        }

        CUSTOM_OP_IMPL(bincount, 1, 1, false, 0, 0) {
            auto values = INPUT_VARIABLE(0);
            
            NDArray *weights = nullptr;

            int maxLength = -1;
            int minLength = 0;
            int maxIndex = values->argMax();
            maxLength = values->e<int>(maxIndex)  + 1;

            if (block.numI() > 0) {
                minLength = nd4j::math::nd4j_max(INT_ARG(0), 0);
                if (block.numI() == 2)
                    maxLength = nd4j::math::nd4j_min(maxLength, INT_ARG(1));
            }

            if (block.width() == 2) { // the second argument is weights
                weights = INPUT_VARIABLE(1);
                REQUIRE_TRUE(values->isSameShape(weights), 0, "bincount: the input and weights shapes should be equals");
            }
            else if (block.width() == 3) { // the second argument is min and the third is max
                auto min= INPUT_VARIABLE(1);
                auto max = INPUT_VARIABLE(2);
                minLength = min->e<int>(0);
                maxLength = max->e<int>(0);
            }
            else if (block.width() > 3) {
                auto min= INPUT_VARIABLE(2);
                auto max = INPUT_VARIABLE(3);
                minLength = min->e<int>(0);
                maxLength = max->e<int>(0);
                weights = INPUT_VARIABLE(1);
                REQUIRE_TRUE(values->isSameShape(weights), 0, "bincount: the input and weights shapes should be equals");

            }
            minLength = nd4j::math::nd4j_max(minLength, 0);
            maxLength = nd4j::math::nd4j_min(maxLength, values->e<int>(maxIndex) + 1);

            auto result = OUTPUT_VARIABLE(0);
            result->assign(0.0f);
             
            helpers::adjustWeights(values, weights, result, minLength, maxLength);

            return Status::OK();
        }

        DECLARE_SHAPE_FN(bincount) {
            auto shapeList = SHAPELIST(); 
            auto in = INPUT_VARIABLE(0);
            nd4j::DataType dtype = DataType::INT32;
            if (block.width() > 1)
                dtype = in->dataType();
            else if (block.numI() > 2)
                dtype = (nd4j::DataType)INT_ARG(2);

            int maxIndex = in->argMax();
            int maxLength = in->e<int>(maxIndex)  + 1;
            int outLength = maxLength;
            if (block.numI() > 0)
                outLength = nd4j::math::nd4j_max(maxLength, INT_ARG(0));

            if (block.numI() > 1) 
                outLength = nd4j::math::nd4j_min(outLength, INT_ARG(1));

            if (block.width() == 3) { // the second argument is min and the third is max
                auto min= INPUT_VARIABLE(1)->e<int>(0);
                auto max = INPUT_VARIABLE(2)->e<int>(0);
                outLength = nd4j::math::nd4j_max(maxLength, min);
                outLength = nd4j::math::nd4j_min(outLength, max);
            }
            else if (block.width() > 3) {
                auto min= INPUT_VARIABLE(2);
                auto max = INPUT_VARIABLE(3);
                outLength = nd4j::math::nd4j_max(maxLength, min->e<int>(0));
                outLength = nd4j::math::nd4j_min(outLength, max->e<int>(0));
            }

            auto newshape = ShapeBuilders::createVectorShapeInfo(dtype, outLength, block.workspace());

            shapeList->push_back(newshape); 
            return shapeList;
        }

    }
}

#endif