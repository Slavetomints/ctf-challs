require 'securerandom'

print "Enter flag: "
flag = gets.chomp

key = SecureRandom.random_number(255) + 1

encrypted = flag.bytes.map { |b| b ^ key }

puts "\n--- Generated C Code ---\n"

puts "#include <stdio.h>\n"

puts "void decrypt_flag() {"
puts "    unsigned char key = 0x#{key.to_s(16).rjust(2,'0')};"

puts "    unsigned char data[] = {"
puts "        #{encrypted.map { |x| "0x#{x.to_s(16).rjust(2,'0')}" }.join(", ")}"
puts "    };"

puts "    int len = #{encrypted.length};"

puts <<~C_CODE

    for(int i = 0; i < len; i++) {
        data[i] ^= key;
    }

    printf("%s\\n", data);
}
C_CODE

puts "\nCall decrypt_flag(); in main()."