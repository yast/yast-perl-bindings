# encoding: utf-8

#
module Yast
  class ReferenceClient < Client
    def main
      Yast.import "Reference"

      @i = 12
      @f = 3.14
      @b = false
      @s = "hu"
      @l = [0, 1, 2, 3, 4]
      @m = { "a" => "x", "b" => "y", "c" => "z", "d" => "123" }

      i_ref = arg_ref(@i)
      Reference.refInt(i_ref)
      @i = i_ref.value
      f_ref = arg_ref(@f)
      Reference.refFloat(f_ref)
      @f = f_ref.value
      b_ref = arg_ref(@b)
      Reference.refBool(b_ref)
      @b = b_ref.value
      s_ref = arg_ref(@s)
      Reference.refString(s_ref)
      @s = s_ref.value
      l_ref = arg_ref(@l)
      Reference.refListInt(l_ref)
      @l = l_ref.value
      m_ref = arg_ref(@m)
      Reference.refMapStringString(m_ref)
      @m = m_ref.value

      Builtins.y2milestone("integer: %1", @i)
      Builtins.y2milestone("float: %1", @f)
      Builtins.y2milestone("boolean: %1", @b)
      Builtins.y2milestone("string: %1", @s)
      Builtins.y2milestone("list: %1", @l)
      Builtins.y2milestone("map: %1", @m)

      nil
    end
  end
end

Yast::ReferenceClient.new.main
