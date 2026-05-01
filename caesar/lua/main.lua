#!/usr/bin/env lua

function Main()
	-- Open the source file
	local source, err_src = io.open(arg[1], "r")
	if not source then
		io.stderr:write("Couldn't open file: " .. err_src .. "\n")
		os.exit(1)
	end
	local pt = source:read("*a") -- Read the source file whole

	local key = tonumber(arg[3]) -- Making sure the key is a number
	if not key then
		io.stderr:write("The key must be an integer value")
		os.exit()
	end

	-- Encrypting and showing the text
	local enc = Encrypt(pt, key)
	print("Plaintext: " .. pt)
	print("Encrypted: " .. enc)

	-- Writing to the output file
	local out, err_out = io.open(arg[2], "w")
	if not out then
		io.stderr:write(err_out .. "\n")
		os.exit(1)
	end
	out:write(enc)
	out:close()
end

function Encrypt(pt, key)
	local enc = {}
	local A, a = string.byte("A"), string.byte("a")

	for i = 1, #pt do
		local c = pt:sub(i, i)
		local byte = string.byte(c)
		if byte >= A and byte <= (A + 25) then
			enc[#enc + 1] = string.char(((byte - A + key) % 26) + A)
		elseif byte >= a and byte <= (a + 25) then
			enc[#enc + 1] = string.char(((byte - a + key) % 26) + a)
		else
			enc[#enc + 1] = c
		end
	end
	return table.concat(enc)
end

function Test()
	local pt = "Hello"
	local keys = { 1, 13, 10 }
	local results = { "Ifmmp", "Uryyb", "Rovvy" }

	print("Testing...")
	for i = 1, #keys do
		local enc = Encrypt(pt, keys[i])
		assert(enc == results[i], "Assertion failed at keys: " .. i)
	end
	for i = 1, #keys do
		local enc = Encrypt(results[i], (keys[i] * -1))
		assert(enc == pt, "Assertion failed at key: " .. i)
	end
	print("All test passed!")
end

-- Making sure sure enough args are provided
if #arg == 1 then
	if arg[1] == "test" then
		Test()
		os.exit(0)
	else
		io.stderr:write("Usage: " .. arg[0] .. " <input> <output> <key>\n")
		os.exit(1)
	end
elseif #arg ~= 3 then
	io.stderr:write("Usage: " .. arg[0] .. " <input> <output> <key>\n")
	os.exit(1)
else
	Main()
end
