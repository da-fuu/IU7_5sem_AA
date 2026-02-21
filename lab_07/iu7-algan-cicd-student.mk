# Обязательно в реализации
# - Цели НЕ менять
# - Зависимости и сценарии поменять на необходимые
TEX_SRCS := $(wildcard ./report/*.tex)


# Собрать на сервере И положить на проверку pdf отчёта
ready/report.pdf : report/report.pdf | ready
	cp $< $@

report/report.pdf : $(TEX_SRCS)
	make -C ./report


ready :
	mkdir -p ./ready

	
# Очистка
.PHONY: clean
clean :
	make -C ./report clean
