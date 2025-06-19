using CSV, Plots, LaTeXStrings, DataFrames, Dates

# default settings for Plots
Plots.default(framestyle=:box,
              titlefontfamily="computer modern",
              legendfontfamily="computer modern",
              guidefontfamily="computer modern",
              markersize=1,
              markerstrokewidth=0)

input_data_filename::String = ARGS[1]

csv_data = CSV.File(input_data_filename; header=3, delim=';', decimal='.')

env_data = dropmissing(DataFrame(csv_data))
rename!(env_data, [:min, :hour, :day, :month, :year, :temp, :hum, :co2])

function parse_dates!(data::DataFrame)
    dates::Vector{DateTime} = []

    for row in eachrow(env_data)
        push!(dates, DateTime(row.year, row.month, row.day, row.hour, row.min))
    end

    data[!, :time] = dates
    data[!, :unixtime] = UInt64.(datetime2unix.(dates))
end

function date_format(unixtime)
    time::DateTime = unix2datetime(unixtime)
    return string(day(time)) * "." * string(month(time)) * "." * string(year(time)) * " " * string(hour(time)) * ":" * string(minute(time))
end

function plot_all(data::DataFrame)
    plotT = plot(data.unixtime, data.temp,
                 seriestype=:scatter,
                 title="Environmental data",
                 ylabel="Temperature [°C]",
                 color="crimson",
                 primary=false,
                 xformatter=date_format,
                 tickfontsize=6)

    plotH = plot(data.unixtime, data.hum,
                 seriestype=:scatter,
                 ylabel="Humidity [%]",
                 color="teal",
                 primary=false,
                 xformatter=date_format,
                 tickfontsize=6)

    plotC = plot(data.unixtime, data.co2,
                 seriestype=:scatter,
                 xlabel="time " * L"t",
                 ylabel="CO2 [ppm]",
                 color="dark blue",
                 primary=false,
                 xformatter=date_format,
                 tickfontsize=6)

    return plot(plotT, plotH, plotC, layout=@layout[a; b; c])
end

parse_dates!(env_data)

p = plot_all(env_data)
Plots.pdf(p, "out/environmental_data.pdf")
