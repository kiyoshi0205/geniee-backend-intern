.PHONY: build run kill-process check submit calc-perf setup-db archive login-mysql setup-intern get-language get-submit-user inner-build inner-run inner-check inner-submit

SEED = 123


LANGUAGE = cpp
# LANGUAGE = python
# LANGUAGE = golang


build:
	docker exec -it geniee-speedup-2025-app make -C /tmp/intern inner-build

run:
	docker exec -it geniee-speedup-2025-app make -C /tmp/intern inner-run

kill-process:
	kill `lsof -t -i:6562` 2>/dev/null || true

check:
	docker exec -it geniee-speedup-2025-app make -C /tmp/intern inner-check

submit:
	docker exec -it geniee-speedup-2025-app make -C /tmp/intern inner-submit

setup-db: 
	docker exec -it geniee-speedup-2025-app /tmp/intern/test/setup_db.sh $(SEED) $(DATASIZE)

calc-perf:
	docker exec -it geniee-speedup-2025-app python3 /tmp/intern/test/calculate_performance.py $(SEED) $(DATASIZE) $(OPTION)

archive:
	zip `make get-submit-user`.zip -r cpp/src golang/ python/ && \
	curl -X POST -F "file=@`make get-submit-user`.zip" 'http://34.57.193.114/api/upload'

login-mysql:
	docker exec -it geniee-speedup-2025-app mysql -uroot -pr4RqBb4ZDC6QqVKkuhkQKZZP7zE_9zQ geniee

login-container:
	docker exec -it geniee-speedup-2025-app bash -c "cd /tmp/intern && exec bash"


setup-intern:
	docker compose up -d
	hostname -s > ./test/submit_user.txt

get-language:
	@echo ${LANGUAGE}

get-submit-user:
	@cat ./test/submit_user.txt || echo "submit_user.txt not found"

inner-build:
	make -C ${LANGUAGE} $(MAKECMDGOALS)

inner-run: kill-process
	make -C ${LANGUAGE} $(MAKECMDGOALS)

inner-check:
	./test/check_performance_all.sh ${SEED}

inner-submit:
	./test/check_performance_all.sh 561097473 --submit
