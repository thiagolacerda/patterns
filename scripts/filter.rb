#!/usr/bin/env ruby
require 'optparse'
require 'ostruct'

class ArgumentParser

    def self.parse(args)
        options = OpenStruct.new

        options.ids = []
        options.file

        optParser = OptionParser.new do |opts|
            opts.banner = 'Usage: filter.rb [options]'

            opts.separator ''
            opts.separator 'Specific options:'

            # Mandatory argument.
            opts.on('-i <ids>', Array, 'array of ids') do |ids|
               options.ids = ids
            end

            opts.on('-f <file_path>', String, 'file path') do |f|
               options.file = f
            end

            opts.on_tail('-h', '--help', 'Show this message') do
                puts opts
                exit
            end
        end

        optParser.parse!(args)
        options
    end
end

def filter(options)
    filtered = []
    File.open(options.file, 'r') do |f|
        f.each_line do |line|
            filtered << line if options.ids.include?(line.split[0])
        end
    end
    File.open('filtered.txt', 'w') do |f|
        filtered.each { |l| f.write(l) }
    end
end

options = ArgumentParser.parse(ARGV)
filter(options)
