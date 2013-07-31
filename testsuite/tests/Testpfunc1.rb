# encoding: utf-8

#
module Yast
  class Testpfunc1Client < Client
    def main
      Yast.import "Testpfunc1"

      Builtins.y2milestone(
        "rxmatch (%1, %2): %3",
        "abracadabra",
        "[a-d]*",
        Testpfunc1.rxmatch("abracadabra", "[a-d]*")
      )
      Builtins.y2milestone(
        "rxmatch (%1, %2): %3",
        "abracadabra",
        "^[a-d]*$",
        Testpfunc1.rxmatch("abracadabra", "^[a-d]*$")
      )

      Builtins.y2milestone(
        "lengths (%1): %2",
        ["one", "two", "three"],
        Testpfunc1.lengths(["one", "two", "three"])
      )

      Builtins.y2milestone(
        "amap (%1): %2",
        { "one" => "two" },
        Testpfunc1.amap({ "one" => "two" })
      )

      nil
    end
  end
end

Yast::Testpfunc1Client.new.main
