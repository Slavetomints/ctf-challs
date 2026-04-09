require 'securerandom'

def generate_flag
  charset = ('a'..'z').to_a +
            ('A'..'Z').to_a +
            ('0'..'9').to_a

  random_part = Array.new(32) { charset.sample }.join

  "STOUTCTF{#{random_part}}"
end

print "How many flags would you like to generate? "
num_flags = gets.chomp.to_i

num_flags.times do
  puts generate_flag
end