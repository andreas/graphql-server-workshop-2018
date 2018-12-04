#! /bin/ruby

def pbcopy(s)
  IO.popen("pbcopy", "w") { |f| f << s }
end

ocaml_slides = open("index.html").read

reason_slides = ocaml_slides.gsub(/```ocaml([^`]*)```/m) do |src|
  pbcopy($1)
  reason_src = `pbpaste | refmt --parse=ml --print=re --print-width=60`
  "```reason\n#{reason_src}\n```"
end

puts reason_slides
