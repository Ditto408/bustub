//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// plan_func_call.cpp
//
// Identification: src/planner/plan_func_call.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <memory>
#include <tuple>
#include "binder/bound_expression.h"
#include "binder/bound_statement.h"
#include "binder/expressions/bound_agg_call.h"
#include "binder/expressions/bound_alias.h"
#include "binder/expressions/bound_binary_op.h"
#include "binder/expressions/bound_column_ref.h"
#include "binder/expressions/bound_constant.h"
#include "binder/expressions/bound_func_call.h"
#include "binder/expressions/bound_unary_op.h"
#include "binder/statement/select_statement.h"
#include "common/exception.h"
#include "common/macros.h"
#include "common/util/string_util.h"
#include "execution/expressions/abstract_expression.h"
#include "execution/expressions/column_value_expression.h"
#include "execution/expressions/constant_value_expression.h"
#include "execution/expressions/string_expression.h"
#include "execution/plans/abstract_plan.h"
#include "fmt/format.h"
#include "planner/planner.h"

namespace bustub {

// NOLINTNEXTLINE
auto Planner::GetFuncCallFromFactory(const std::string &func_name, std::vector<AbstractExpressionRef> args)
    -> AbstractExpressionRef {
  if (func_name == "lower" || func_name == "upper") {
    if (args.size() == 0 || args.size() > 1) {
      throw Exception(fmt::format("func {} should take 1 parameter.", func_name));
    }
    AbstractExpressionRef arg = args.front();
    StringExpressionType string_type;
    if (func_name == "lower")
      string_type = StringExpressionType ::Lower;
    else
      string_type = StringExpressionType ::Upper;
    StringExpression stringExpression = StringExpression(arg, string_type);
    return std::make_shared<StringExpression>(stringExpression);
  } else {
    throw Exception(fmt::format("func call {} not supported in planner yet", func_name));
  }
}

}  // namespace bustub
