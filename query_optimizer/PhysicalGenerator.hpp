/**
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
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_GENERATOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_GENERATOR_HPP_

#include <memory>
#include <unordered_map>
#include <vector>

#include "query_optimizer/LogicalToPhysicalMapper.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/strategy/Strategy.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Converts logical plans to physical plans.
 */
class PhysicalGenerator : public LogicalToPhysicalMapper {
 public:
  /**
   * @brief Constructor
   */
  PhysicalGenerator() {
    createStrategies();
  }

  /**
   * @brief Destructor.
   */
  ~PhysicalGenerator() {}

  /**
   * @return The generated physical plan.
   */
  const physical::PhysicalPtr& physical_plan() const { return physical_plan_; }

  /**
   * @brief Generates and optimizes a physical plan for \p logical_plan.
   *
   * @param logical_plan The logical plan to be converted to a physical plan.
   * @return The physical plan.
   */
  physical::PhysicalPtr generatePlan(const logical::LogicalPtr &logical_plan);

  /**
   * @brief Sets the best physical plan for \p logical_plan is \p physical_plan.
   *
   * @param logical_plan Logical plan.
   * @param physical_plan The physical plan for the logical plan.
   */
  void setBestPhysicalForLogical(const logical::LogicalPtr &logical_plan,
                                 const physical::PhysicalPtr &physical_plan);

  physical::PhysicalPtr createOrGetPhysicalFromLogical(
      const logical::LogicalPtr &logical_plan) final;

 private:
  /**
   * @brief Creates an initial physical plan for \p logical_plan.
   *        It tries each strategy to the logical plan in order,
   *        and the physical plan generated by the first strategy that can
   *        be applied is chosen as the best initial physical plan.
   *
   *
   * @param logical_plan The logical plan to be converted to a physical plan.
   * @return The initial physical plan.
   */
  physical::PhysicalPtr generateInitialPlan(
      const logical::LogicalPtr &logical_plan);

  /**
   * @brief Creates strategies for different types of physical nodes.
   */
  void createStrategies();

  /**
   * @brief Applies physical rules to the initial physical plan.
   *
   * @return The optimized physical plan.
   */
  physical::PhysicalPtr optimizePlan();

  std::vector<std::unique_ptr<strategy::Strategy>> strategies_;

  /**
   * @brief Maps a logical plan to its initial physical plan.
   *        We memorize every generated sub-physical plan in this map.
   *        This map is currently useless, because each logical node is
   *        visited only once and its physical plan won't be retrieved
   *        from the map. This is used to allow strategy tests to change
   *        the plans created for a logical plan by putting intended plans
   *        into the map so that the generator won't apply a strategy to
   *        create a plan for a logical node. It also makes testing on
   *        different strategies to be relatively independent. A strategy test
   *        only checks one type of physical plans, and manually provides other
   *        types of physical plans that are not interested. This map will become
   *        more useful when we have a cost model to evaluate the cost of a
   *        physical plan. The best physical plan for a logical subplan may be shared
   *        in multiple physical plans during the plan enumeration.
   */
  std::unordered_map<logical::LogicalPtr, physical::PhysicalPtr> logical_to_physical_map_;

  /**
   * @brief The complete physical plan.
   */
  physical::PhysicalPtr physical_plan_;

  DISALLOW_COPY_AND_ASSIGN(PhysicalGenerator);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_GENERATOR_HPP_ */
