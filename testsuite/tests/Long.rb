# encoding: utf-8

# #127896, #75798
module Yast
  class LongClient < Client
    def main
      Yast.import "Long"
      Builtins.y2milestone("3*2**30: %1", Long.three_billion)
      Builtins.y2milestone("3*2**30: %1 (class)", Long.three_billion_c)
      Builtins.y2milestone("3*2**40: %1", Long.three_trillion)
      Builtins.y2milestone("3*2**40: %1 (class)", Long.three_trillion_c)

      # 3 fits to UV, not IV
      @lm = [-7, -1, 1, 3, 7]
      Builtins.foreach(@lm) do |m|
        Builtins.y2milestone("%1 * 2**30: %2", m, Long.big_num(m, 3))
        Builtins.y2milestone("%1 * 2**30: %2 class", m, Long.big_num_c(m, 3))
      end

      @g = 1024 * 1024 * 1024
      Builtins.foreach(@lm) do |m|
        Builtins.y2milestone(
          "loop %1 * 2**30: %2",
          m,
          Long.loop(Ops.multiply(m, @g))
        )
        Builtins.y2milestone(
          "loop %1 * 2**30: %2 class",
          m,
          Long.loop_c(Ops.multiply(m, @g))
        )
      end

      nil
    end
  end
end

Yast::LongClient.new.main
