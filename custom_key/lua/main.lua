#!/usr/bin/env lua

FileInput = {}

function Main()
	local src, err_src = io.open(arg[1], "r")
	if not src then
		io.stderr:write("Couldn't open file: " .. err_src .. "\n")
		os.exit(1)
	end
	local all_data = src:read("*a")
	src:close()
	Parse_file(all_data)
	local enc = Encrypt()
	print("Plaintext: " .. FileInput[2])
	print("Encrypted: " .. enc)

	local out, err_out = io.open(arg[2], "w")
	if not out then
		io.stderr:write("Error: " .. err_out .. "\n")
		os.exit(1)
	end
	out:write(enc)
	out:close()
end

function Parse_file(all_data)
	local kw = "KEY"
	local key_len = 26
	for word in all_data:gmatch("%w+") do
		if word ~= kw then
			io.stderr:write("The input file must have the keyword: 'KEY'\n")
			os.exit(1)
		end
		break
	end

	local key_total_len = kw:len() + 2 + key_len
	local k_str = all_data:sub(kw:len() + 2, key_total_len)
	local k_table = {}
	for i in k_str:gmatch(".") do
		table.insert(k_table, i:upper())
	end
	local text = all_data:sub(key_total_len + 1, all_data:len())
	table.insert(FileInput, k_table)
	table.insert(FileInput, text)
end

function Encrypt()
	local enc = {}
	local A, a = string.byte("A"), string.byte("a")

	for c in FileInput[2]:gmatch(".") do
		local k = c
		local byte_c = string.byte(c)
		if byte_c >= A and byte_c <= (A + 25) then
			k = FileInput[1][(byte_c - (A - 1))]
		elseif byte_c >= a and byte_c <= (a + 25) then
			k = FileInput[1][(byte_c - (a - 1))]:lower()
		end
		table.insert(enc, k)
	end
	return table.concat(enc)
end

function Test()
	local text = "Hello"
	local results_enc = { "Ifmmp", "Uryyb", "Rovvy" }
	-- local results_dec = { "Khoor", "Hello", "Byffi" }
	local keys_enc = {
		"BCDEFGHIJKLMNOPQRSTUVWXYZA",
		"NOPQRSTUVWXYZABCDEFGHIJKLM",
		"KLMNOPQRSTUVWXYZABCDEFGHIJ",
	}
	local keys_dec = {
		"ZABCDEFGHIJKLMNOPQRSTUVWXY",
		"NOPQRSTUVWXYZABCDEFGHIJKLM",
		"QRSTUVWXYZABCDEFGHIJKLMNOP",
	}
	local k_table_enc = { {}, {}, {} }
	for i = 1, #keys_enc do
		for j in keys_enc[i]:gmatch(".") do
			table.insert(k_table_enc[i], j)
		end
	end
	local k_table_dec = { {}, {}, {} }
	for i = 1, #keys_dec do
		for j in keys_dec[i]:gmatch(".") do
			table.insert(k_table_dec[i], j)
		end
	end

	-- To add key as a table in FileInput
	FileInput = { "", text }
	for i = 1, #keys_enc do
		FileInput[1] = k_table_enc[i]
		local enc = Encrypt()
		assert(enc == results_enc[i], "Assertion failed at key no.: " .. i)
	end
	for i = 1, #keys_dec do
		FileInput[1] = k_table_dec[i]
		FileInput[2] = results_enc[i]
		local enc = Encrypt()
		assert(enc == text, "Assertion failed at key no.: " .. i)
	end
	print("All tests passed")
end

if #arg == 1 then
	if arg[1] == "test" then
		print("Testing...")
		Test()
		os.exit(0)
	else
		io.stderr:write("Usage: " .. arg[0] .. " <input> <output>\n")
	end
elseif #arg <= 2 then
	io.stderr:write("Usage: " .. arg[0] .. " <input> <output>\n")
	os.exit(1)
else
	Main()
end
