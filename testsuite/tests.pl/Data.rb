# encoding: utf-8

#
require "yast"

module Yast
  class DataClass < Module
    def loop(a)
      a = deep_copy(a)
      Builtins.y2milestone("loop: %1", a)
      deep_copy(a)
    end

    publish :function => :loop, :type => "any (any)"
  end

  Data = DataClass.new
end
